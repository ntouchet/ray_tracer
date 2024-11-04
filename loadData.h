#pragma once

#include "light.h"
#include <iostream>
#include <tira/parser.h>
#include "hittableList.h"
#include <glm/glm.hpp>

lighting setLights(tira::parser& scene_file);


hittableList setHittables(tira::parser& scene_file);


tira::camera setCamera(tira::parser& scene_file);


glm::vec3 setBackground(tira::parser& scene_file);


int* setImage(tira::parser& scene_file);




