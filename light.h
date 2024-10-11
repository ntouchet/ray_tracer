#pragma once
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include "rays.h"
#include "tira/graphics/camera.h"


// Light class
class pointSource
{
public:
    pointSource() {}

    pointSource(glm::vec3 position, glm::vec3 color) : pos(position), col(color) {}


    const glm::vec3 position() const { return pos; }
    const glm::vec3 color() const { return col; }

    ray generateRay(glm::vec3 point)
    {
        //no need to normalize because you wont need the distance from the origin to the intersect for lighting intersections.
        ray r(point, glm::normalize(pos-point));
        return r;
    }

    glm::vec3 pos;
    glm::vec3 col;
};


