#include <glm/ext/vector_float3.hpp>
#include <tira/image.h>
#include <glm/vec3.hpp>
#include <cmath>
#include <tira/parser.h>
#include <tira/graphics/camera.h>
#include "intersect.h"
#include "calculateColor.h"
#include "scene.h"
#include "rays.h"

// TODO: please put these all in their own files when you get the chance






int main()
{
    Scene basic_scene("./scenes/spheramid.scene");
    tira::image<unsigned char> I(1000,1000,3);


    int red;
    int green;
    int blue;
    ray r;
    r.origin = basic_scene.Camera.position();
    intersect p;
    float x_pixel_position;
    float y_pixel_position;
    for(int y = 0; y<1000;y++)
    {
        for(int x = 0; x<1000;x++)
        {
            x_pixel_position = (static_cast<float>(x)/1000 - 0.5f);
            y_pixel_position = (static_cast<float>(y)/1000 - 0.5f);
            r.vector = basic_scene.Camera.ray(x_pixel_position,y_pixel_position);
            for(int i = 0; i < basic_scene.number_of_spheres; i++)
            {
                p = sphereIntersect(basic_scene.spheres[i],r);
                if(p.intersect_state)
                {
                    calculateColor(p, basic_scene.lights, basic_scene.number_of_lights, basic_scene.spheres, basic_scene.number_of_spheres);
                    red = static_cast<int>(255*p.intensity[0]*basic_scene.spheres[i].color[0]);
                    green = static_cast<int>(255*p.intensity[1]*basic_scene.spheres[i].color[1]);
                    blue = static_cast<int>(255*p.intensity[2]*basic_scene.spheres[i].color[2]);

//                   NOTE: why do I use x and y here but x and y _pixel_position in the else statement, but it works.
                    I(x,y,0)=red;
                    I(x,y,1)=green;
                    I(x,y,2)=blue;
                }
                else
                {
                    I(x_pixel_position,y_pixel_position,0)=0;
                    I(x_pixel_position,y_pixel_position,1)=0;
                    I(x_pixel_position,y_pixel_position,2)=0;
                }
            }
        }
    }

    I.save("output.bmp");

    return 0;
}
