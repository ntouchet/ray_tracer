
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <sstream>
#include <iostream>

#include "triangle.h"
#include "hittable.h"
#include "rays.h"


triangle setTriangle(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2){
    triangle t;
    t.m_v[0] = V0;
    t.m_v[1] = V1;
    t.m_v[2] = V2;

    //Calculate Normal
    t.m_v0v1 = t.m_v[1] - t.m_v[0];
    t.m_v1v2 = t.m_v[2] - t.m_v[1];
    t.m_v2v0 = t.m_v[0] - t.m_v[2];
    glm::vec3 n = glm::cross(t.m_v0v1,t.m_v1v2);
    t.m_normal = glm::normalize(n);
    t.m_color = {1,1,1};


    //Calculate bounding sphere parameters
    t.m_centroid = (t.m_v[0]+t.m_v[1]+t.m_v[2])/3.0f;

    for (int i = 0; i<3 ; i++)
    {
        float r = glm::length(t.m_centroid - t.m_v[i]);
        if (r > t.m_radius)
            t.m_radius = r;
    }

    return t;
}


    std::string triangeStr(triangle t) 
    {
        std::stringstream ss;
        ss<<"---Triangle---\n"<<"V0: "<<glm::to_string(t.m_v[0]) << "V1: "<<glm::to_string(t.m_v[1])<< "V2: "<<glm::to_string(t.m_v[2]) << "\nNormal: "<<t.m_radius<<"\nColor: "<<glm::to_string(t.m_color)<<"\n";
        return ss.str();
    }

    bool triangleHit(triangle tr, const ray& r, float max_dist, hitRecord& rec) 
    {
        //std::cout << "Ray:\nDirection -> x " << r.direction().x << ", y " << r.direction().y << ", z " << r.direction().y;
        glm::vec3 l = tr.m_centroid - r.origin();
        float l_squared = glm::dot(l,l);
        float radius_squared = tr.m_radius*tr.m_radius;

        //Test if the sphere is behind the camera
        float s = glm::dot(l,r.direction());
        if(s<0)
        {
            //std::cout << "The bounding sphere is behind the camera\n";
            return false;
        }

        //Test if the ray intersects the sphere
        float m_squared = l_squared - (s*s);
        if(m_squared>radius_squared)
        {
            return false;
        }

        //Begin Plane Intersect test
        float denom = glm::dot(tr.m_normal, r.direction());

        if (fabs(denom) < 1e-6)
        {
            //std::cout << "The ray is parallel to the plane\n";
            return false;
        }

        float t = glm::dot(tr.m_normal,(tr.m_v[0]-r.origin()))/denom;
        if (t < 0 || t > max_dist)
        {
            //std::cout << "The distance to the point is out of range\n";
            return false;
        }

        //Find intersect point on plane and test if point is inside triangle
        glm::vec3 p = r.at(t);

        //To the left of first line
        glm::vec3 pa = p-tr.m_v[0];
        float cross1 = glm::dot(glm::cross(tr.m_v0v1,pa),tr.m_normal);
        if (cross1 < 0)
        {
            //std::cout << "Not Left of first line\n";
            return false;
        }

        //To the left of second line
        glm::vec3 pb = p-tr.m_v[1];
        float cross2 = glm::dot(glm::cross(tr.m_v1v2,pb),tr.m_normal);
        if (cross2 < 0)
        {
            //std::cout << "Not Left of second line\n";
            return false;
        }

        //To the left of third line
        glm::vec3 pc = p-tr.m_v[2];
        float cross3 = glm::dot(glm::cross(tr.m_v2v0,pc),tr.m_normal);
        if (cross3 < 0)
        {
            //std::cout << "Not Left of third line\n";
            return false;
        }

        rec.t = t;
        rec.p = p;
        rec.normal = tr.m_normal;
        rec.color = tr.m_color;

        //std::cout << "Intersect detected\n";

        return true;
    }

