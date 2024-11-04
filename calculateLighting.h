#pragma once
#include "hittableList.h"
#include "hittable.h"
#include "light.h"

glm::vec3 onHitShadow(const hittableList& world, hitRecord& rec, float intersect_distance_maximum, int number_of_lights, pointSource* lights);
