#pragma once
#include "scene.h"
#include "intersect.h"

void calculateColor(intersect& p, light* lights, int number_of_lights, sphere* spheres, int number_of_spheres);
