#pragma once

#include "light.h"
#include <iostream>
#include <tira/parser.h>
#include <glm/glm.hpp>
#include "triangle.h"
#include "sphere.h"
#include "plane.h"

lighting setLights(tira::parser& scene_file);

void loadSpheres(tira::parser& scene_file, sphere* spheres, const int nSpheres);

void loadMesh(tira::parser& mesh_file, triangle* triangles, const int nTriangles);

void loadPlanes(tira::parser& scene_file, plane* planes, const int nPlanes);

tira::camera setCamera(tira::parser& scene_file);

glm::vec3 setBackground(tira::parser& scene_file);

int* setImage(tira::parser& scene_file);





