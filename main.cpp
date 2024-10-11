#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <memory>
#include <tira/image.h>
#include <glm/vec3.hpp>
#include <cmath>
#include <tira/parser.h>
#include <tira/graphics/camera.h>
#include <limits>

#include "hittable.h"
#include "hittableList.h"
#include "sphere.h"
#include "plane.h"
#include "rays.h"



int main(int argc, char* argv[])
{

    tira::image<unsigned char> I(1000,1000,3);

    tira::camera cam;
    glm::vec3 camera_positon = {0,5,-10};
    glm::vec3 camer_lookat = {0,-3,0};
    glm::vec3 camera_up = {0,1,1};
    float camera_fov = 45;
    cam.position(camera_positon);
    cam.lookat(camer_lookat);
    cam.up(camera_up);
    cam.fov(camera_fov);

    float max_val = std::numeric_limits<float>::max();

    hittableList world;
    std::shared_ptr<hittable> big_back = std::make_shared<sphere>(glm::vec3(0.5,0.5,0.5),glm::vec3(-3,-4,-3),.5f);
    std::shared_ptr<hittable> small_front = std::make_shared<sphere>(glm::vec3(.9,0,0.9),glm::vec3(-2,-4,-3),.5f);
    std::shared_ptr<hittable> bottom_plane = std::make_shared<plane>(glm::vec3(0,-4.5,0),glm::vec3(0,-1,0),glm::vec3(.9,.5,.2));
    world.add(big_back);
    world.add(small_front);
    world.add(bottom_plane);

    float red {};
    float blue {};
    float green {};

    float x_pixel_position;
    float y_pixel_position;
    hitRecord rec;
    for(int y = 0; y<1000;y++)
    {
        for(int x = 0; x<1000;x++)
        {
            x_pixel_position = (static_cast<float>(x)/1000 - 0.5f);
            y_pixel_position = (static_cast<float>(y)/1000 - 0.5f);
            ray r(cam.position(), cam.ray(x_pixel_position,y_pixel_position));
            if(world.hit(r,max_val,rec))
            {

//                if(auto Plane = std::dynamic_pointer_cast<const plane>(rec.object_ptr))
//                    std::cout << "A plane Was Hit\n";
//                if(auto Sphere = std::dynamic_pointer_cast<const sphere>(rec.object_ptr))
//                    std::cout << "A sphere Was Hit\n";
                red = static_cast<int>(255*rec.color.r);
                green = static_cast<int>(255*rec.color.g);
                blue = static_cast<int>(255*rec.color.b);
//               NOTE: why do I use x and y here but x and y _pixel_position in the else statement, but it works.
                I(x,y,0)=red;
                I(x,y,1)=green;
                I(x,y,2)=blue;
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

    return 0;
}
