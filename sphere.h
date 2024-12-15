#pragma once

#include <glm/glm.hpp>
#include <string>

struct sphere{
    float m_radius;
    glm::vec3 m_center;
    glm::vec3 m_color;
};

std::string sphere_str(sphere s);
