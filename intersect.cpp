#include "intersect.h"
#include <cmath>
#include <glm/glm.hpp>


intersect sphereIntersect(sphere test_sphere, ray test_ray)
{
    intersect final_intersect;

    //Test if the camera is inside the sphere
    glm::vec3 distance_to_sphere = test_sphere.position - test_ray.origin;
    float distance_to_sphere_squared = glm::dot(distance_to_sphere,distance_to_sphere);
    float radius_squared = test_sphere.radius*test_sphere.radius;
    if(distance_to_sphere_squared < radius_squared)
    {
        final_intersect.intersect_state = false;
        return final_intersect;
    }

    //Test if the sphere is behind the camera
    float distance_projected_onto_ray = glm::dot(distance_to_sphere,test_ray.vector);
    if(distance_projected_onto_ray<0)
    {
        final_intersect.intersect_state = false;
        return final_intersect;
    }

    //Test is the ray intersects the sphere
    float m_squared = distance_to_sphere_squared - distance_projected_onto_ray*distance_projected_onto_ray;
    if(m_squared>radius_squared)
    {
        final_intersect.intersect_state = false;
        return final_intersect;
    }

    //Return the position of the intersect and the normal of the sphere at that point
    float q_squared = radius_squared - m_squared;
    final_intersect.intersect_point = test_ray.origin + (distance_projected_onto_ray - std::sqrt(q_squared))*test_ray.vector;
    final_intersect.intersect_point_normal = glm::normalize(final_intersect.intersect_point - test_sphere.position);

    return final_intersect;

}
