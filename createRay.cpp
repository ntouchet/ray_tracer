#include "rays.h"



ray createRay(glm::vec3 origin, glm::vec3 second_point)
{
    ray r;
    r.origin = origin;
    glm::vec3 direction_vector = glm::normalize(second_point - origin);
    return r;
}
