#include "kernel.h"
#include <iostream>
#include "calculateLighting.h"
#include "hittable.h"
#include "light.h"

void kernel(int start_row, int end_row, int* image_data, tira::camera cam, const struct plane* pl, const struct triangle* tr, const struct sphere* sp, const struct lighting l,
            const int y_res, const int x_res, float intersect_distance_maximum, int nSpheres, int nPlanes, int nTriangles)
{
    float x_pixel_position;
    float y_pixel_position;
    glm::vec3 color;
    hitRecord rec;
    int number_of_rows = end_row-start_row;

    for(int y = 0; y < number_of_rows; y++)
    {
        int row_in_image = start_row+y;
        for(int x = 0; x<x_res;x++)
        {
            x_pixel_position = (static_cast<float>(x)/(float)y_res - 0.5f);
            y_pixel_position = (static_cast<float>(row_in_image)/(float)x_res - 0.5f);
            ray r(cam.position(), glm::normalize(cam.ray(x_pixel_position,y_pixel_position)));

            if(closestHit(r, intersect_distance_maximum, rec, tr, sp, pl, nSpheres, nPlanes, nTriangles))
            {
                //std::cout << rec.object_ID << "\n";
                //std::cout << rec.t << "\n";
                color = onHitShadow(sp, tr, pl, rec, intersect_distance_maximum, l.number_of_lights, l.lights, nSpheres, nTriangles, nPlanes);

                image_data[3*(row_in_image*y_res+x)+0]=static_cast<int>(255*color.r);
                image_data[3*(row_in_image*y_res+x)+1]=static_cast<int>(255*color.g);
                image_data[3*(row_in_image*y_res+x)+2]=static_cast<int>(255*color.b);
            }
            else
            {
                image_data[3*(row_in_image*y_res+x)+0]=0;
                image_data[3*(row_in_image*y_res+x)+1]=0;
                image_data[3*(row_in_image*y_res+x)+2]=0;
            }
        }
    }
}
