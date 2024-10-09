#pragma once
#include <glm/vec3.hpp>
#include "scene.h"
#include "rays.h"

// This class holds relevant information about an intersect point
// normal vector (for lighting), intersect point, weather or not there was an intersection
// and the intensity of the RGB lighting at that point.
struct intersect
{
    bool intersect_state;
    glm::vec3 intersect_point = {0,0,0};
    glm::vec3 intersect_point_normal = {0,0,0};
    float intensity[3] = {0,0,0};
    int object_index;

};

intersect sphereIntersect(sphere o, ray r);

intersect shortestIntersect(sphere* o, int number_of_spheres, ray r);
