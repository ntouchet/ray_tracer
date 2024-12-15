
#include "hittable.h"
#include "rays.h"
#include <glm/vec3.hpp>
#include "sphere.h"
#include "triangle.h"
#include "plane.h"






bool closestHit(const ray& r, float max_val, hitRecord& rec, const triangle* triangles, const sphere* spheres, const plane* planes, int nSpheres, int nPlanes, int nTriangles){
    hitRecord temporary_record;
    bool hit_anything = false;
    float closest_so_far = max_val;
    int i = 0;

    for (size_t i = 0; i<nSpheres; i++)
    {
        if (sphereHit(spheres[i], r, closest_so_far, temporary_record))
        {
            //std::cout << "an object was hit\n";
            hit_anything = true;
            closest_so_far = temporary_record.t;
            rec = temporary_record;
            rec.object_ID = i;
        }
    }
    for (size_t i = 0; i<nPlanes; i++)
    {
        if (planeHit(planes[i], r, closest_so_far, temporary_record))
        {
            //std::cout << "an object was hit\n";
            hit_anything = true;
            closest_so_far = temporary_record.t;
            rec = temporary_record;
            rec.object_ID = i;
        }
    }
    for (size_t i = 0; i<nTriangles; i++)
    {
        if (triangleHit(triangles[i], r, closest_so_far, temporary_record))
        {
            //std::cout << "an object was hit\n";
            hit_anything = true;
            closest_so_far = temporary_record.t;
            rec = temporary_record;
            rec.object_ID = i;
        }
    }
    return hit_anything;
}





bool lightHit(const ray& r, float max_val, hitRecord& current_point, const triangle* triangles, const sphere* spheres, const plane* planes, int nSpheres, int nPlanes, int nTriangles)
{
    hitRecord temporary_record;

    for (size_t i = 0; i<nSpheres; i++)
    {
        if (sphereHit(spheres[i], r, max_val, temporary_record) && i != current_point.object_ID)
        {
            return true;
        }
    }
    for (size_t i = 0; i<nPlanes; i++)
    {
        if (planeHit(planes[i], r, max_val, temporary_record) && i != current_point.object_ID)
        {
            return true;
        }
    }
    for (size_t i = 0; i<nTriangles; i++)
    {
        if (triangleHit(triangles[i], r, max_val, temporary_record) && i != current_point.object_ID)
        {
            return true;
        }
    }
    return false;
}

