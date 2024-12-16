#pragma once 
#include <cuda_runtime.h>

__global__ void kernelPerPixel(unsigned char *image_data, struct camera2 * cam,
                               const struct plane *pl,
                               const struct triangle *tr,
                               const struct sphere *sp,
                               const struct pointSource *lights,
                               const int y_res, const int x_res,
                               float intersect_distance_maximum, int nSpheres,
                               int nPlanes, int nTriangles, int nLights); 
