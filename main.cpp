#include <iostream>
#include <tira/parser.h>
#include <thread>
#include <tira/image.h>
#include "loadData.h"
#include "calculateLighting.h"
#include "light.h"
#include "hittable.h"
#include "hittableList.h"
#include "kernel.h"



int main(int argc, char* argv[])
{
    char* file = argv[1];
    int number_of_threads = atoi(argv[2]);


    tira::parser scene_file(file);
    lighting l = setLights(scene_file);
    hittableList world = setHittables(scene_file);
    int* resolution = setImage(scene_file);
    tira::camera cam = setCamera(scene_file);
    glm::vec3 background_color = setBackground(scene_file);
    float intersect_distance_maximum = std::numeric_limits<float>::max();


    if(number_of_threads==1)
    {

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
                if(world.hit(r,intersect_distance_maximum,rec))
                {
                    color = onHitShadow(world, rec, intersect_distance_maximum, l.number_of_lights, l.lights);
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
        int* image_data = new int[resolution[0]*resolution[1]*3];

        tira::image<unsigned char>I(resolution[0],resolution[1],3);
        int strip_size = y_res/number_of_threads;

        std::thread* threads = new std::thread[number_of_threads];
        for(int i = 0; i<number_of_threads; i++){
            int start_index = i*strip_size;
            int end_index = (i==number_of_threads-1) ? y_res : (i+1)*strip_size;
            threads[i] = std::thread(&kernel, start_index, end_index, image_data, cam, world, l, resolution[0], resolution[1], intersect_distance_maximum);
        }

        for(int i = 0; i<number_of_threads; i++){
            threads[i].join();
        }

        delete[] threads;

        for(int y = 0; y<resolution[1];y++)
        {
            for(int x = 0; x<resolution[0];x++)
            {
                    I(x,y,0)=image_data[3*(y*resolution[0]+x)+0];
                    I(x,y,1)=image_data[3*(y*resolution[0]+x)+1];
                    I(x,y,2)=image_data[3*(y*resolution[0]+x)+2];
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
