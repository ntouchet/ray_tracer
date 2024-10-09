#include "intersect.h"
#include <algorithm>

// This function assigns a value to the intensity member of the intersect struct.
// It takes the intersect and a list of all lights.
void calculateColor(intersect& p, light* lights, int number_of_lights, sphere* spheres, int number_of_spheres)
{
    float intensity {};
    float intensity_r {};
    float intensity_g {};
    float intensity_b {};
    ray light_line;
    intersect sphere_intersect;

    for(int i = 0; i<number_of_lights; i++)
    {
        glm::vec3 r = glm::normalize(lights[i].position - p.intersect_point);
        intensity = std::max(glm::dot(r,p.intersect_point_normal),static_cast<float>(0));
        if(intensity != 0)
        {
            light_line = createRay(p.intersect_point,lights[i].position);
            for(int j = 0; j < number_of_spheres; j++)
            {
                sphere_intersect = sphereIntersect(spheres[j], light_line);
                if(sphere_intersect.intersect_state)
                {
                    intensity = 0;
                    break;
                }

            }

        }

        intensity_r += intensity*lights[i].color[0];
        intensity_g += intensity*lights[i].color[1];
        intensity_b += intensity*lights[i].color[2];
    }
    p.intensity[0] = std::clamp(intensity_r,0.0f,1.0f);
    p.intensity[1] = std::clamp(intensity_g,0.0f,1.0f);
    p.intensity[2] = std::clamp(intensity_b,0.0f,1.0f);

//    std::cout << "p in function: " << p.color[0] << "\n";
//    p.color[1] = std::min(intensity_g,static_cast<float>(1))*color[1];
//    p.color[2] = std::min(intensity_b,static_cast<float>(1))*color[2];
}
