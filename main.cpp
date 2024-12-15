#include <iostream>
#include <tira/parser.h>
#include <thread>
#include <tira/image.h>
#include <vector>
#include "loadData.h"
#include "calculateLighting.h"
#include "light.h"
#include "hittable.h"
#include "kernel.h"
#include "triangle.h"
#include <string>



int main(int argc, char* argv[])
{
    char* file = argv[1];
    int number_of_threads = atoi(argv[2]);


    tira::parser scene_file(file);
    lighting l = setLights(scene_file);
    int* resolution = setImage(scene_file);
    int nSpheres = scene_file.count("sphere");
    int nPlanes = scene_file.count("plane");
    int nMeshes = scene_file.count("mesh");

    int nTriangles = 0;
    struct triangle* triangles = NULL;
    if(nMeshes>0){
        tira::parser mesh_file("./scenes/"+scene_file.get<std::string>("mesh",0));
        nTriangles = mesh_file.count("f");
        triangles = (struct triangle*)malloc(nTriangles*sizeof(struct triangle));
        loadMesh(mesh_file, triangles, nTriangles);
    }

    struct sphere* spheres = NULL;
    if(nSpheres>0){
        spheres = (struct sphere*)malloc(nSpheres*sizeof(struct sphere));
        loadSpheres(scene_file, spheres, nSpheres);
    }

    struct plane* planes = NULL;
    if(nPlanes>0){
        planes = (struct plane*)malloc(nPlanes*sizeof(struct plane));
        loadPlanes(scene_file, planes, nPlanes);
    }

    tira::camera cam = setCamera(scene_file);
    glm::vec3 background_color = setBackground(scene_file);
    float intersect_distance_maximum = std::numeric_limits<float>::max();


    if(number_of_threads==0)
    {

        std::cout<<"Run Regular\n";
        auto start = std::chrono::high_resolution_clock::now();


        tira::image<unsigned char> I(resolution[0],resolution[1],3);

        glm::vec3 color = {0,0,0};
        float x_pixel_position {};
        float y_pixel_position {};
        hitRecord rec;
        //std::cout << m_cam << "\n";
        for(int y = 0; y<resolution[1];y++)
        {
            int imag_y = (resolution[1]-y);
            for(int x = 0; x<resolution[0];x++)
            {
                x_pixel_position = (static_cast<float>(x)/(float)resolution[0] - 0.5f);
                y_pixel_position = (static_cast<float>(imag_y)/(float)resolution[1] - 0.5f);
                ray r(cam.position(), glm::normalize(cam.ray(x_pixel_position,y_pixel_position)));
                //std::cout << "(" << x << ", " << y << ")";
                //std::cout << "position: (" << r.origin().x << ", " << r.origin().y << ", " << r.origin().z << ")" << " direction: (" << r.direction().x << ", " << r.direction().y << ", " << r.direction().z << ")\n";
                if(closestHit(r, intersect_distance_maximum, rec, triangles, spheres, planes, nSpheres, nPlanes, nTriangles))
                {
                    color = onHitShadow(spheres, triangles , planes , rec, intersect_distance_maximum, l.number_of_lights, l.lights, nSpheres, nTriangles, nPlanes);
                    //std::cout << "r " << color.r << " " << "g " << color.g << " " << "b " << color.b << "\n";

                    I(x,y,0)=static_cast<int>(255*color.r);
                    I(x,y,1)=static_cast<int>(255*color.g);
                    I(x,y,2)=static_cast<int>(255*color.b);
                }
                else
                {
                    I(x,y,0)=0;
                    I(x,y,1)=0;
                    I(x,y,2)=0;
                }

            }
        }

        I.save("output.bmp");

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
        std::cout << "---Timing---\n"<< "Total Trace Time (not including loading file): " << ((double)duration/(1'000.0));
    }
    else
    {
        auto start = std::chrono::high_resolution_clock::now();
        int y_res = resolution[1];
        int* image_data = new int[resolution[0]*resolution[1]*3]();

        tira::image<unsigned char>I(resolution[0],resolution[1],3);
        int strip_size = y_res/number_of_threads;

//
//        int** thread_data = new int*[number_of_threads];
//        for(int i = 0 ; i<number_of_threads; i++){
//            thread_data[i] = new int[strip_size*resolution[0]*3]();
//        }

        std::vector<std::thread> threads;
        for(int i = 0; i<number_of_threads; i++){
            //if(i==0){
                int start_index = i*strip_size;
                int end_index = (i==number_of_threads-1) ? y_res : (i+1)*strip_size;
                threads.emplace_back(&kernel, start_index, end_index, image_data, cam, planes, triangles, spheres, l, resolution[0], resolution[1],
                                     intersect_distance_maximum, nSpheres, nPlanes, nTriangles);
            //}
        }

        for(int i = 0; i<number_of_threads; i++){
            //if(i==0){
                threads[i].join();
            //}
        }

        for(int y = 0; y<resolution[1]; y++){
            int y_i = resolution[1]-y-1;
            for(int x = 0; x<resolution[0]; x++){
                I(x,y,0) = (unsigned char)image_data[(y_i*resolution[0]+x)*3+0];
                I(x,y,1) = (unsigned char)image_data[(y_i*resolution[0]+x)*3+1];
                I(x,y,2) = (unsigned char)image_data[(y_i*resolution[0]+x)*3+2];
            }
        }

        I.save("output.bmp");

        delete[] image_data;

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
        std::cout << "---Timing---\n"<< "Total Trace Time (not including loading file): " << ((double)duration/(1'000.0));
    }

    return 0;
}
