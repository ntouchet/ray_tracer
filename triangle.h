#pragma once

#include <glm/glm.hpp>
#include <string>

struct triangle{
    glm::vec3 m_v[3];
    glm::vec3 m_normal;
    glm::vec3 m_centroid;
    float m_radius = -std::numeric_limits<float>::infinity();
    glm::vec3 m_color = {1,1,1};
    glm::vec3 m_v0v1;
    glm::vec3 m_v1v2;
    glm::vec3 m_v2v0;
};

triangle setTriangle(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2);

std::string triangeStr(triangle t);
