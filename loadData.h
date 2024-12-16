#pragma once
#include <iostream>
#include <tira/graphics/camera.h>
#include <tira/parser.h>
#include <glm/glm.hpp>
#include "structs.h"
#include "camera.h"

void setLights(tira::parser& scene_file, pointSource* lights);

void loadSpheres(tira::parser& scene_file, sphere* spheres, const int nSpheres);

void loadMesh(tira::parser& mesh_file, triangle* triangles, const int nTriangles);

void loadPlanes(tira::parser& scene_file, plane* planes, const int nPlanes);

camera setCamera(tira::parser& scene_file);

glm::vec3 setBackground(tira::parser& scene_file);

int* setImage(tira::parser& scene_file);





