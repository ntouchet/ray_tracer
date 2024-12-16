#pragma once

#include <glm/glm.hpp>

struct hitRecord {
  glm::vec3 p;
  glm::vec3 normal;
  int object_ID;
  float t;
  glm::vec3 color;
};

struct triangle{
    glm::vec3 m_v[3];
    glm::vec3 m_normal;
    glm::vec3 m_centroid;
    float m_radius = -std::numeric_limits<float>::infinity();
    glm::vec3 m_color = {1,1,1};
    glm::vec3 m_v0v1;
    glm::vec3 m_v1v2;
    glm::vec3 m_v2v0;
};

struct sphere{
    float m_radius;
    glm::vec3 m_center;
    glm::vec3 m_color;
};

struct plane{
    glm::vec3 m_point;
    glm::vec3 m_normal;
    glm::vec3 m_color;
};

struct ray
{
    glm::vec3 orig;
    glm::vec3 dir;
};

struct camera2{
    glm::vec3 _view;					// view vector (direction the camera is looking)
    glm::vec3 _position;				// camera location (eye point)
    glm::vec3 _up;						// vector pointing upwards (from the camera location)
    float _focus;						// focal length of the camera
    float _fov;							// camera field of view
    float _imagedist;					// distance to a unit-sized image plane
};

struct pointSource
{
    glm::vec3 pos;
    glm::vec3 col;
};


struct lighting
{
    pointSource* lights;
    int number_of_lights;
};
