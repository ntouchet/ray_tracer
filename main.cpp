#include "scene.h"
#include "intersect.h"
#include "rays.h"
#include <tira/image.h>

int main()
{
    Scene basic_scene("./scenes/basic.scene");
    tira::image<unsigned char>I(basic_scene.resolution[0],basic_scene.resolution[1],3);


    ray r;
    intersect p;
    for(int x = 0; x<basic_scene.resolution[0];x++)
    {
        for(int y = 0; y<basic_scene.resolution[1];y++)
        {
            r.vector = basic_scene.Camera.ray((x/basic_scene.resolution[0]-0.5),(y/basic_scene.resolution[1]-0.5));
            r.origin = basic_scene.Camera.position();
            for(int i = 0; i < basic_scene.number_of_spheres; i++)
            {
                p = sphereIntersect(basic_scene.spheres[i],r);
                if(p.intersect_state == true)
                    I(x,y,0)=255;
            }
        }
    }

    I.save("output.bmp");

    return 0;
}
