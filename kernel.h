#pragma once
#include "light.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"

void kernel(int start_row, int end_row, int* image_data, tira::camera cam, const struct plane* pl, const struct triangle* tr, const struct sphere* sp, const struct lighting l,
            const int y_res, const int x_res, float intersect_distance_maximum, int nSpheres, int nPlanes, int nTriangles);
