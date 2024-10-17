#pragma once
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include "hittable.h"
#include "rays.h"


class triangle : public hittable
{
public:
    triangle(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2) : m_v{V0, V1, V2}
    {
        //Calculate Normal
        m_v0v1 = m_v[1] - m_v[0];
        m_v1v2 = m_v[2] - m_v[1];
        m_v2v0 = m_v[0] - m_v[2];
        glm::vec3 n = glm::cross(m_v1v2,m_v0v1);
        m_normal = glm::normalize(n);


        //Calculate bounding sphere parameters
        m_centroid = (m_v[0]+m_v[1]+m_v[2])/3.0f;

        for (int i = 0; i<3 ; i++)
        {
            float r = glm::length(m_centroid - m_v[i]);
            if (r > m_radius)
                m_radius = r;
        }

    }

    bool hit(const ray& r, float max_dist, hitRecord& rec) const override
    {
        //std::cout << "Ray:\nDirection -> x " << r.direction().x << ", y " << r.direction().y << ", z " << r.direction().y;
        glm::vec3 l = m_centroid - r.origin();
        float l_squared = glm::dot(l,l);
        float radius_squared = m_radius*m_radius;

        //Test if the sphere is behind the camera
        float s = glm::dot(l,r.direction());
        if(s<0)
        {
            return false;
        }

        //Test if the ray intersects the sphere
        float m_squared = l_squared - (s*s);
        if(m_squared>radius_squared)
        {
            return false;
        }

        //Begin Plane Intersect test
        float denom = glm::dot(m_normal, r.direction());

        if (denom < 1e-6)
            return false;

        float t = -glm::dot(m_normal,(m_v[0]-r.origin()))/denom;
        if (t < 0 || t > max_dist)
            return false;

        //Find intersect point on plane and test if point is inside triangle
        glm::vec3 p = r.at(t);

        //To the left of first line
        glm::vec3 pa = p-m_v[0];
        float cross1 = glm::dot(glm::cross(m_v0v1,pa),m_normal);
        if (cross1 < 0)
            return false;

        //To the left of second line
        glm::vec3 pb = p-m_v[1];
        float cross2 = glm::dot(glm::cross(m_v1v2,pb),m_normal);
        if (cross2 < 0)
            return false;

        //To the left of third line
        glm::vec3 pc = p-m_v[2];
        float cross3 = glm::dot(glm::cross(m_v2v0,pc),m_normal);
        if (cross3 < 0)
            return false;

        rec.t = t;
        rec.p = p;
        rec.normal = m_normal;
        rec.color = m_color;

        return true;
    }

private:
    glm::vec3 m_v[3];
    glm::vec3 m_normal;
    glm::vec3 m_centroid;
    float m_radius {0};
    glm::vec3 m_color = {1,1,1};
    glm::vec3 m_v0v1;
    glm::vec3 m_v1v2;
    glm::vec3 m_v2v0;

};
