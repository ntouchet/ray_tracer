#pragma once


#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <tira/graphics/camera.h>

// Class that represents a ray
struct ray
{
    glm::vec3 origin;
    glm::vec3 vector;
};

ray createRay(glm::vec3 origin, glm::vec3 second_point);
