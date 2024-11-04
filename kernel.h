#pragma once
#include "light.h"
#include "hittableList.h"

void kernel(int start_row, int end_row, int* image_data, tira::camera cam, hittableList world, const lighting& l, const int y_res, const int x_res, float intersect_distance_maximum);
