#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <mutex>
#include <sstream>
#include <string>
#include <chrono>

#include "hittable.h"
#include "rays.h"
#include "timing.h"


class sphere : public hittable
{
public:
    sphere(glm::vec3 color, glm::vec3 center, float radius) : m_color(color), m_center(center), m_radius(radius) {}

    std::string str() const override
    {
        std::stringstream ss;
        ss<<"---Sphere---\n"<<"Center: "<<glm::to_string(m_center)<<"\nRadius: "<<m_radius<<"\nColor: "<<glm::to_string(m_color)<<"\n";
        return ss.str();
    }


    bool hit(const ray& r, float max_dist, hitRecord& rec) const override
    {

        //Test if the camera is inside the sphere
        //std::cout << "Ray:\nDirection -> x " << r.direction().x << ", y " << r.direction().y << ", z " << r.direction().y;
        glm::vec3 l = m_center - r.origin();
        float l_squared = glm::dot(l,l);
        float radius_squared = m_radius*m_radius;
        if(l_squared < radius_squared)
        {
  //          std::cout << "INSIDE SPHERE\n";
            return false;
        }

        //Test if the sphere is behind the camera
        float s = glm::dot(l,r.direction());
        if(s<0)
        {
 //           std::cout << "SPHERE BEHIND CAMERA\n";
            return false;
        }

        //Test if the ray intersects the sphere
        float m_squared = l_squared - (s*s);
        if(m_squared>radius_squared)
        {
//            std::cout << "M is greater than R\n";
            return false;
        }

        //Return the position of the intersect and the normal of the sphere at that point
        float q_squared = radius_squared - m_squared;
        float q = std::sqrt(q_squared);
        float t = s - q; 
        if(t > max_dist)
        {
            //std::cout << "SPHERE DISTANCE TOO LARGE\n";
            return false;
        }
        rec.t = t;
        rec.p = r.at(t);
        rec.normal = glm::normalize((rec.p - m_center));
        rec.color = m_color;

        return true;
    }

private:
    float m_radius;
    glm::vec3 m_center;
    glm::vec3 m_color;

};
