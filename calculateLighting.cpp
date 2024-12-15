#include "rays.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "hittable.h"
#include "calculateLighting.h"
#include "light.h"
#include <algorithm>

glm::vec3 onHitShadow(const sphere* sp, const triangle* tr, const plane* pl,  hitRecord& rec, float intersect_distance_maximum, int number_of_lights, pointSource* lights, const int nSpheres,
                      const int nTriangles, const int nPlanes)
{
    glm::vec3 color_intensity {0.0,0.0,0.0};
    hitRecord temp_record;
    float intensity;
    glm::vec3 color;
    ray lighting_ray;

    for(int i = 0; i < number_of_lights; i++)
    {
        lighting_ray = lights[i].generateRay(rec.p);
        //std::cout << "LIGHT VECTOR: x " << lighting_ray.direction().x << ", y " << lighting_ray.direction().y << ", z " << lighting_ray.direction().z << "\n";
        //std::cout << "NORMAL VECTOR: x " << rec.normal.x << ", y " << rec.normal.y << ", z " << rec.normal.z << "\n";
        intensity = std::max(glm::dot(lighting_ray.direction(),rec.normal),static_cast<float>(0));
        //std::cout << "intensity: " << intensity << "\n";
        if (intensity > static_cast<float>(0))
        {
            if(!lightHit(lighting_ray,intersect_distance_maximum,rec, tr, sp,pl,nSpheres,nPlanes,nTriangles))
            {
                color_intensity.r += intensity*lights[i].col.r;
                color_intensity.g += intensity*lights[i].col.g;
                color_intensity.b += intensity*lights[i].col.b;
            }
        }

    }
    color = {std::clamp(color_intensity.r,0.0f,1.0f)*rec.color.r, std::clamp(color_intensity.g,0.0f,1.0f)*rec.color.g, std::clamp(color_intensity.b,0.0f,1.0f)*rec.color.b};


    return color;

}
