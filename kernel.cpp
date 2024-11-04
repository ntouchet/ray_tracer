#include "kernel.h"
#include <iostream>
#include "calculateLighting.h"
#include "light.h"
void kernel(int start_row, int end_row, int* image_data, tira::camera cam,const hittableList world, const lighting& l, const int x_res, const int y_res, const float intersect_distance_maximum)
{

    int x_pixel_position;
    int y_pixel_position;
    glm::vec3 color;
    hitRecord rec;

    for(int y = start_row; y < end_row; y++)
    {
        int imag_y = (y_res-y);
        for(int x = 0; x<x_res;x++)
        {
            x_pixel_position = (static_cast<float>(x)/(float)y_res - 0.5f);
            y_pixel_position = (static_cast<float>(imag_y)/(float)x_res - 0.5f);
            ray r(cam.position(), glm::normalize(cam.ray(x_pixel_position,y_pixel_position)));

            if(world.hit(r,intersect_distance_maximum,rec))
            {
                //std::cout << rec.object_ID << "\n";
                //std::cout << rec.t << "\n";
                color = onHitShadow(world, rec, intersect_distance_maximum, l.number_of_lights, l.lights);

                image_data[3*(y*y_res+x)+0]=static_cast<int>(255*color.r);
                image_data[3*(y*y_res+x)+1]=static_cast<int>(255*color.g);
                image_data[3*(y*y_res+x)+2]=static_cast<int>(255*color.b);
            }
            else
            {
                image_data[3*(y*y_res+x)+0]=0;
                image_data[3*(y*y_res+x)+1]=0;
                image_data[3*(y*y_res+x)+2]=0;
            }
        }
    }
}
