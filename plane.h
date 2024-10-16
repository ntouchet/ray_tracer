#pragma once

#include "hittable.h"
#include <glm/glm.hpp>
#include <cmath>



class plane : public hittable 
{
public:
    plane(glm::vec3 point, glm::vec3 normal, glm::vec3 color) : m_point(point), m_normal(normal), m_color(color) {}

    bool hit(const ray& r, float max_dist, hitRecord& rec) const override
    {
        float denom = glm::dot(m_normal, r.direction());

        if (std::fabs(denom) < 1e-6)
            return false;

        float t = glm::dot(m_normal,(m_point-r.origin()))/denom;
        if (t < 0 || t > max_dist)
            return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.normal = m_normal;
        rec.color = m_color;
        return true;
    }

private:
    glm::vec3 m_point;
    glm::vec3 m_normal;
    glm::vec3 m_color;
};
