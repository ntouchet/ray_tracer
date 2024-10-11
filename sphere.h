#pragma once
#include "hittable.h"
#include "rays.h"
#include <glm/glm.hpp>


class sphere : public hittable
{
public:
    sphere(glm::vec3 color, glm::vec3 center, float radius) : color(color), center(center), radius(radius) {}

    bool hit(const ray& r, float max_dist, hitRecord& rec) const override
    {
        //Test if the camera is inside the sphere
        glm::vec3 d = center - r.origin();
        float distance_to_sphere_squared = glm::dot(d,d);
        float radius_squared = radius*radius;
        if(distance_to_sphere_squared < radius_squared)
            return false;

        //Test if the sphere is behind the camera
        float distance_projected_onto_ray = glm::dot(d,r.direction());
        if(distance_projected_onto_ray<0)
            return false;

        //Test is the ray intersects the sphere
        float m_squared = distance_to_sphere_squared - distance_projected_onto_ray*distance_projected_onto_ray;
        if(m_squared>radius_squared)
            return false;

        //Return the position of the intersect and the normal of the sphere at that point
        float q_squared = radius_squared - m_squared;
        float q = std::sqrt(q_squared);
        float t = distance_projected_onto_ray - q; 
        if(t > max_dist)
            return false;
        rec.t = t;
        rec.p = r.origin() + (distance_projected_onto_ray - q*r.direction());
        rec.normal = glm::normalize(rec.p - center);
        rec.color = color;

        return true;
    }

private:
    float radius;
    glm::vec3 center;
    glm::vec3 color;

};
