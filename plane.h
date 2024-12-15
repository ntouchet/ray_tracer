#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <cmath>
#include <string>




struct plane{
    glm::vec3 m_point;
    glm::vec3 m_normal;
    glm::vec3 m_color;
};

std::string planeStr(plane p);
