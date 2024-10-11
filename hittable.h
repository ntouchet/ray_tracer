#pragma once

#include "rays.h"
#include <glm/vec3.hpp>
#include <memory>

class hitRecord;

class hittable
{
public: 
    virtual ~hittable() = default;
    virtual bool hit(const ray& r, float max_distance, hitRecord& rec) const=0;
};


class hitRecord 
{
public:
    glm::vec3 p;
    glm::vec3 normal;
    std::shared_ptr<const hittable> object_ptr;
    float t;
    glm::vec3 color;
};
