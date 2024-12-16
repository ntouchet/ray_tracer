
#include <cuda_runtime_api.h>
#include <driver_types.h>
#include <vector_types.h>
#define HANDLE_ERROR(err) (HandleError(err, __FILE__, __LINE__))
#include "camera.h"
#include "kernel.h"
#include "loadData.h"
#include "structs.h"
#include <chrono>
#include <cuda_runtime.h>
#include <iostream>
#include <string>
#include <tira/image.h>
#include <tira/parser.h>

__constant__ float intersect_distance_maximum =
    std::numeric_limits<float>::max();
__constant__ int nTriangles_gpu;
__constant__ int nSpheres_gpu;
__constant__ int nPlanes_gpu;
__constant__ int nLights_gpu;
__constant__ int y_res;
__constant__ int x_res;

static void HandleError(cudaError_t err, const char *file, int line) {
  if (err != cudaSuccess)
    std::cout << cudaGetErrorString(err) << " in " << file << " at line "
              << line << "\n";
}

int main(int argc, char *argv[]) {
  char *file = argv[1];

  tira::parser scene_file(file);
  int nLights = scene_file.count("light");
  struct pointSource *lights =
      (struct pointSource *)malloc(nLights * sizeof(struct pointSource));
  setLights(scene_file, lights);
  struct pointSource *lights_gpu = NULL;

  HANDLE_ERROR(cudaMalloc(&lights_gpu, sizeof(struct pointSource)));
  HANDLE_ERROR(cudaMemcpy(lights_gpu, lights,
                          nLights * sizeof(struct pointSource),
                          cudaMemcpyHostToDevice));
  int *resolution = setImage(scene_file);
  int nSpheres = scene_file.count("sphere");
  int nPlanes = scene_file.count("plane");
  int nMeshes = scene_file.count("mesh");
  HANDLE_ERROR(cudaMemcpyToSymbol(&nSpheres_gpu, &nSpheres, 1 * sizeof(int)));
  HANDLE_ERROR(cudaMemcpyToSymbol(&nPlanes_gpu, &nPlanes, 1 * sizeof(int)));
  HANDLE_ERROR(cudaMemcpyToSymbol(&nLights_gpu, &nPlanes, 1 * sizeof(int)));
  HANDLE_ERROR(cudaMemcpyToSymbol(&x_res, &resolution[0], 1 * sizeof(int)));
  HANDLE_ERROR(cudaMemcpyToSymbol(&y_res, &resolution[1], 1 * sizeof(int)));

  // cuda setup
  int *device_count;
  HANDLE_ERROR(cudaGetDeviceCount(device_count));
  if (*device_count == 0) {
    std::cout << "There are no available CUDA devices\n";
    return 0;
  } else
    std::cout << "There are " << *device_count << "CUDA devices\n";

  int *cuda_device;
  HANDLE_ERROR(cudaGetDevice(cuda_device));
  cudaDeviceProp *properties;
  HANDLE_ERROR(cudaGetDeviceProperties(properties, *cuda_device));

  int nTriangles = 0;
  struct triangle *triangles_gpu = NULL;
  struct triangle *triangles = NULL;
  if (nMeshes > 0) {
    tira::parser mesh_file("./scenes/" +
                           scene_file.get<std::string>("mesh", 0));
    nTriangles = mesh_file.count("f");
    HANDLE_ERROR(
        cudaMemcpyToSymbol(&nTriangles_gpu, &nTriangles, 1 * sizeof(int)));
    triangles = (struct triangle *)malloc(nTriangles * sizeof(struct triangle));
    loadMesh(mesh_file, triangles, nTriangles);
    HANDLE_ERROR(
        cudaMalloc(&triangles_gpu, nTriangles * sizeof(struct triangle)));
    HANDLE_ERROR(cudaMemcpy(triangles_gpu, triangles,
                            (size_t)nTriangles * sizeof(struct triangle),
                            cudaMemcpyHostToDevice));
  }

  struct sphere *spheres = NULL;
  struct sphere *spheres_gpu = NULL;
  if (nSpheres > 0) {
    spheres = (struct sphere *)malloc(nSpheres * sizeof(struct sphere));
    loadSpheres(scene_file, spheres, nSpheres);
    HANDLE_ERROR(cudaMalloc(&spheres_gpu, nSpheres * sizeof(struct sphere)));
    HANDLE_ERROR(cudaMemcpy(spheres_gpu, spheres,
                            (size_t)nSpheres * sizeof(struct sphere),
                            cudaMemcpyHostToDevice));
  }

  struct plane *planes = NULL;
  struct plane *planes_gpu = NULL;
  if (nPlanes > 0) {
    planes = (struct plane *)malloc(nPlanes * sizeof(struct plane));
    loadPlanes(scene_file, planes, nPlanes);
    HANDLE_ERROR(cudaMalloc(&planes_gpu, nPlanes * sizeof(struct plane)));
    HANDLE_ERROR(cudaMemcpy(planes_gpu, planes,
                            (size_t)nPlanes * sizeof(struct plane),
                            cudaMemcpyHostToDevice));
  }

  camera cam = setCamera(scene_file);
  struct camera2 *cam2;
  struct camera2 *cam2_gpu;
  cam2->_up = cam._up;
  cam2->_fov = cam._fov;
  cam2->_view = cam._view;
  cam2->_position = cam._position;
  cam2->_imagedist = cam._imagedist;
  cam2->_focus = cam._focus;
  HANDLE_ERROR(cudaMalloc(&cam2, nPlanes * sizeof(struct camera2)));
  HANDLE_ERROR(cudaMemcpy(cam2_gpu, cam2, 1 * sizeof(struct camera2),
                          cudaMemcpyHostToDevice));
  glm::vec3 background_color = setBackground(scene_file);

  auto start = std::chrono::high_resolution_clock::now();

  tira::image<unsigned char> I(resolution[0], resolution[1], 3);

  unsigned char *image_data =
      new unsigned char[resolution[0] * resolution[1] * 3]();
  unsigned char *image_data_gpu = NULL;

  HANDLE_ERROR(cudaMalloc(&image_data_gpu, resolution[0] * resolution[1] * 3));

  int block_side_length = std::sqrt(properties->maxThreadsPerBlock);
  int block_side_length2 = block_side_length;
  int nBlocks_y = resolution[1] / block_side_length + 1;
  int nBlocks_x = resolution[0] / block_side_length + 1;

  kernelPerPixel<<<dim3(nBlocks_x, nBlocks_y),
                   dim3(block_side_length, block_side_length2)>>>(
      image_data_gpu, cam2_gpu, planes_gpu, triangles_gpu, spheres_gpu,
      lights_gpu, y_res, x_res, intersect_distance_maximum, nSpheres, nPlanes,
      nTriangles, nLights);

  cudaDeviceSynchronize();

  HANDLE_ERROR(
      cudaMemcpy(image_data_gpu, image_data,
                 resolution[0] * resolution[1] * 3 * sizeof(unsigned char),
                 cudaMemcpyDeviceToHost));

  for (int y = 0; y < resolution[1]; y++) {
    int y_i = resolution[1] - y - 1;
    for (int x = 0; x < resolution[0]; x++) {
      I(x, y, 0) = image_data[(y_i * resolution[0] + x) * 3 + 0];
      I(x, y, 1) = image_data[(y_i * resolution[0] + x) * 3 + 1];
      I(x, y, 2) = image_data[(y_i * resolution[0] + x) * 3 + 2];
    }
  }

  I.save("output.bmp");

  delete[] image_data;
  cudaFree(image_data_gpu);
  cudaFree(cam2_gpu);
  cudaFree(planes_gpu);
  cudaFree(spheres_gpu);
  cudaFree(triangles_gpu);
  cudaFree(lights_gpu);

  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(endTime - start)
          .count();
  std::cout << "---Timing---\n"
            << "Total Trace Time (not including loading file): "
            << ((double)duration / (1'000.0));

  return 0;
}
