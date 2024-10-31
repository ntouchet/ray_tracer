#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>

#include "hittable.h"
#include "rays.h"
#include "plane.h"
#include "timing.h"

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

    bool hit(const ray& r, float max_val, hitRecord& rec, timingInfo& total_lighting_time) const override
    {
        auto start = std::chrono::high_resolution_clock::now();
        hitRecord temporary_record;
        bool hit_anything = false;
        auto closest_so_far = max_val;
        int i = 0;

        for (const auto& object : objects)
        {
            if (object->hit(r, closest_so_far, temporary_record, total_lighting_time))
            {
                //std::cout << "an object was hit\n";
                hit_anything = true;
                closest_so_far = temporary_record.t;
                rec = temporary_record;
                rec.object_ID = i;
            }
            i++;
        }


        auto endTime = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
        total_lighting_time.total_ray_intersect += (long long)stop;

        return hit_anything;

    }


    bool lightHit(const ray& r, float max_val, hitRecord& current_point, timingInfo& total_lighting_time)
    {
        auto start = std::chrono::high_resolution_clock::now();
        hitRecord temporary_record;
        int i = 0;

        for (const auto& object : objects)
        {
            if (object->hit(r, max_val, temporary_record, total_lighting_time) && i!=current_point.object_ID)
            {
                auto endTime = std::chrono::high_resolution_clock::now();
                auto stop = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
                total_lighting_time.lighting_intersect += (long long)stop;

                return true;
            }
            i++;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
        total_lighting_time.lighting_intersect += (long long)stop;

        return false;
    }

};

