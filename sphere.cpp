#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <sstream>
#include <string>

#include "hittable.h"
#include "rays.h"
#include "sphere.h"



std::string sphere_str(sphere s)
{
    std::stringstream ss;
    ss<<"---Sphere---\n"<<"Center: "<<glm::to_string(s.m_center)<<"\nRadius: "<< s.m_radius << "\nColor: " << glm::to_string(s.m_color)<<"\n";
    return ss.str();
}


bool sphereHit(const struct sphere& sph, const ray& r, float max_dist, struct hitRecord& rec)
{

    //Test if the camera is inside the sphere
    //std::cout << "Ray:\nDirection -> x " << r.direction().x << ", y " << r.direction().y << ", z " << r.direction().y;
    glm::vec3 l = sph.m_center - r.origin();
    float l_squared = glm::dot(l,l);
    float radius_squared = sph.m_radius*sph.m_radius;
    if(l_squared < radius_squared)
    {
        return false;
    }

    //Test if the sphere is behind the camera
    float s = glm::dot(l,r.direction());
    if(s<0)
    {
//      std::cout << "SPHERE BEHIND CAMERA\n";
        return false;
    }

    //Test if the ray intersects the sphere
    float m_squared = l_squared - (s*s);
    if(m_squared>radius_squared)
    {//            std::cout << "M is greater than R\n";
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
    rec.normal = glm::normalize((rec.p - sph.m_center));
    rec.color = sph.m_color;

    return true;
}


