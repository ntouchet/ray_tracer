#pragma once
#include <glm/vec3.hpp>
#include "scene.h"
#include "rays.h"

struct intersect
{
    bool intersect_state;
    glm::vec3 intersect_point = {0,0,0};
    glm::vec3 intersect_point_normal = {0,0,0};

};

intersect sphereIntersect(sphere o, ray r);
