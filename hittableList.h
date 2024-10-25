#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

#include "hittable.h"
#include "rays.h"
#include "plane.h"

using std::make_shared;
using std::shared_ptr;

class hittableList : public hittable 
{
public:
    std::vector<shared_ptr<hittable> > objects;

    hittableList() {}
    hittableList(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    std::string str() const override
    {
        std::stringstream ss;
        ss<<"---Hittables---"<<"Number of Hittables: "<<objects.size()<<"\n";
        return ss.str();
    }

    bool hit(const ray& r, float max_val, hitRecord& rec) const override
    {
        hitRecord temporary_record;
        bool hit_anything = false;
        auto closest_so_far = max_val;
        int i = 0;

        for (const auto& object : objects)
        {
            if (object->hit(r, closest_so_far, temporary_record))
            {
                //std::cout << "an object was hit\n";
                hit_anything = true;
                closest_so_far = temporary_record.t;
                rec = temporary_record;
                rec.object_ID = i;
            }
            i++;
        }

        return hit_anything;
    }
    bool lightHit(const ray& r, float max_val, hitRecord& current_point) 
    {
        hitRecord temporary_record;
        int i = 0;

        for (const auto& object : objects)
        {
            if (object->hit(r, max_val, temporary_record) && i!=current_point.object_ID)
            {
                return true;
            }
            i++;
        }

        return false;
    }

};

