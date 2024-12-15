#pragma once

#include "triangle.h"
#include "sphere.h"
#include "plane.h"
#include "rays.h"
#include <glm/vec3.hpp>

struct hitRecord 
{
    glm::vec3 p;
    glm::vec3 normal;
    int object_ID;
    float t;
    glm::vec3 color;
};


bool triangleHit(triangle tr, const ray& r, float max_dist, hitRecord& rec);
bool planeHit(plane p, const ray& r, float max_dist, hitRecord& rec);
bool sphereHit(const struct sphere& sph, const ray& r, float max_dist, hitRecord& rec);

bool closestHit(const ray& r, float max_val, hitRecord& rec, const triangle* triangles, const sphere* spheres, const plane* planes, int nSpheres, int nPlanes, int nTriangles);
bool lightHit(const ray& r, float max_val, hitRecord& current_point, const triangle* triangles, const sphere* spheres, const plane* planes, int nSpheres, int nPlanes, int nTriangles);
