#pragma once

#include "hittable.h"
#include <glm/glm.hpp>
#include <cmath>



class plane : public hittable 
{
public:
    plane(glm::vec3 point, glm::vec3 normal, glm::vec3 color) : point(point), normal(normal), color(color) {}

    bool hit(const ray& r, float max_dist, hitRecord& rec) const override
    {
        float denom = glm::dot(normal, r.direction());

        if (fabs(denom) < 1e-6)
            return false;

        float t = glm::dot(normal,point-r.origin())/denom;
        if (t < 0 || t > max_dist)
            return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.normal = normal;
        rec.color = color;
        return true;
    }

private:
    glm::vec3 point;
    glm::vec3 normal;
    glm::vec3 color;
};
