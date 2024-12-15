
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <cmath>
#include <sstream>
#include <string>

#include "hittable.h"
#include "plane.h"
#include "timing.h"





    std::string planeStr(plane p) 
    {
        std::stringstream ss;
        ss<<"---Plane---\n"<<"Point: "<<glm::to_string(p.m_point)<<"\nNormal: "<<glm::to_string(p.m_normal)<<"\nColor: "<<glm::to_string(p.m_color)<<"\n";
        return ss.str();
    }

    bool planeHit(plane p, const ray& r, float max_dist, hitRecord& rec)
    {
        float denom = glm::dot(p.m_normal, r.direction());

        if (std::fabs(denom) < 1e-6)
            return false;

        float t = glm::dot(p.m_normal,(p.m_point-r.origin()))/denom;
        if (t < 0 || t > max_dist)
            return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.normal = p.m_normal;
        rec.color = p.m_color;
        return true;
    }

