#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <cmath>
#include <sstream>
#include <string>

#include "hittable.h"
#include "timing.h"



class plane : public hittable 
{
public:
    plane(glm::vec3 point, glm::vec3 normal, glm::vec3 color) : m_point(point), m_normal(normal), m_color(color) {}

    std::string str() const override
    {
        std::stringstream ss;
        ss<<"---Plane---\n"<<"Point: "<<glm::to_string(m_point)<<"\nNormal: "<<glm::to_string(m_normal)<<"\nColor: "<<glm::to_string(m_color)<<"\n";
        return ss.str();
    }

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
