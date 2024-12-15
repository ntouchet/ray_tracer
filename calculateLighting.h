#pragma once
#include "hittable.h"
#include "light.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"

glm::vec3 onHitShadow(const sphere* sp, const triangle* tr, const plane* pl,  hitRecord& rec, float intersect_distance_maximum, int number_of_lights, pointSource* lights, const int nSpheres,
                      const int nTriangles, const int nPlanes);
