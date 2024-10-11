#pragma once
#include <iostream>
#include "hittable.h"
#include "rays.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "plane.h"

using std::make_shared;
using std::shared_ptr;

class hittableList : public hittable 
{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittableList() {}
    hittableList(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    bool hit(const ray& r, float max_val, hitRecord& rec) const override
    {
        hitRecord temporary_record;
        bool hit_anything = false;
        auto closest_so_far = max_val;
        bool closest_is_plane;

        for (const auto& object : objects)
        {
            closest_is_plane =false; 
            if (object->hit(r, closest_so_far, temporary_record))
            {
                hit_anything = true;
                closest_so_far = temporary_record.t;
                rec = temporary_record;
                rec.object_ptr = object; 
            }
        }

        return hit_anything;
    }
};

