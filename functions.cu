#include "kernel.h"
#include "structs.h"
#include <__clang_cuda_builtin_vars.h>
#include <algorithm>
#include <driver_types.h>
#include <glm/glm.hpp>

__device__ glm::vec3 rayAt(const struct ray &r, float t) {
  return r.orig + t * r.dir;
};

__device__ bool triangleHit(triangle tr, const ray &r, float max_dist,
                            hitRecord &rec) {
  // std::cout << "Ray:\nDirection -> x " << r.direction().x << ", y " <<
  // r.direction().y << ", z " << r.direction().y;
  glm::vec3 l = tr.m_centroid - r.orig;
  float l_squared = glm::dot(l, l);
  float radius_squared = tr.m_radius * tr.m_radius;

  // Test if the sphere is behind the camera
  float s = glm::dot(l, r.dir);
  if (s < 0) {
    // std::cout << "The bounding sphere is behind the camera\n";
    return false;
  }

  // Test if the ray intersects the sphere
  float m_squared = l_squared - (s * s);
  if (m_squared > radius_squared) {
    return false;
  }

  // Begin Plane Intersect test
  float denom = glm::dot(tr.m_normal, r.dir);

  if (fabs(denom) < 1e-6) {
    // std::cout << "The ray is parallel to the plane\n";
    return false;
  }

  float t = glm::dot(tr.m_normal, (tr.m_v[0] - r.orig)) / denom;
  if (t < 0 || t > max_dist) {
    // std::cout << "The distance to the point is out of range\n";
    return false;
  }

  // Find intersect point on plane and test if point is inside triangle
  glm::vec3 p = rayAt(r, t);

  // To the left of first line
  glm::vec3 pa = p - tr.m_v[0];
  float cross1 = glm::dot(glm::cross(tr.m_v0v1, pa), tr.m_normal);
  if (cross1 < 0) {
    // std::cout << "Not Left of first line\n";
    return false;
  }

  // To the left of second line
  glm::vec3 pb = p - tr.m_v[1];
  float cross2 = glm::dot(glm::cross(tr.m_v1v2, pb), tr.m_normal);
  if (cross2 < 0) {
    // std::cout << "Not Left of second line\n";
    return false;
  }

  // To the left of third line
  glm::vec3 pc = p - tr.m_v[2];
  float cross3 = glm::dot(glm::cross(tr.m_v2v0, pc), tr.m_normal);
  if (cross3 < 0) {
    // std::cout << "Not Left of third line\n";
    return false;
  }

  rec.t = t;
  rec.p = p;
  rec.normal = tr.m_normal;
  rec.color = tr.m_color;

  // std::cout << "Intersect detected\n";

  return true;
}
__device__ bool planeHit(plane p, const ray &r, float max_dist,
                         hitRecord &rec) {
  float denom = glm::dot(p.m_normal, r.dir);

  if (std::fabs(denom) < 1e-6)
    return false;

  float t = glm::dot(p.m_normal, (p.m_point - r.orig)) / denom;
  if (t < 0 || t > max_dist)
    return false;

  rec.t = t;
  rec.p = rayAt(r, t);
  rec.normal = p.m_normal;
  rec.color = p.m_color;
  return true;
}
__device__ bool sphereHit(const struct sphere &sph, const ray &r,
                          float max_dist, struct hitRecord &rec) {

  // Test if the camera is inside the sphere
  // std::cout << "Ray:\nDirection -> x " << r.direction().x << ", y " <<
  // r.direction().y << ", z " << r.direction().y;
  glm::vec3 l = sph.m_center - r.orig;
  float l_squared = glm::dot(l, l);
  float radius_squared = sph.m_radius * sph.m_radius;
  if (l_squared < radius_squared) {
    return false;
  }

  // Test if the sphere is behind the camera
  float s = glm::dot(l, r.dir);
  if (s < 0) {
    //      std::cout << "SPHERE BEHIND CAMERA\n";
    return false;
  }

  // Test if the ray intersects the sphere
  float m_squared = l_squared - (s * s);
  if (m_squared >
      radius_squared) { //            std::cout << "M is greater than R\n";
    return false;
  }

  // Return the position of the intersect and the normal of the sphere at that
  // point
  float q_squared = radius_squared - m_squared;
  float q = std::sqrt(q_squared);
  float t = s - q;
  if (t > max_dist) {
    // std::cout << "SPHERE DISTANCE TOO LARGE\n";
    return false;
  }
  rec.t = t;
  rec.p = rayAt(r, t);
  rec.normal = glm::normalize((rec.p - sph.m_center));
  rec.color = sph.m_color;

  return true;
}

__device__ bool closestHit(const ray &r, float max_val, hitRecord &rec,
                           const triangle *triangles, const sphere *spheres,
                           const plane *planes, int nSpheres, int nPlanes,
                           int nTriangles) {
  hitRecord temporary_record;
  bool hit_anything = false;
  float closest_so_far = max_val;
  int i = 0;

  for (size_t i = 0; i < nSpheres; i++) {
    if (sphereHit(spheres[i], r, closest_so_far, temporary_record)) {
      // std::cout << "an object was hit\n";
      hit_anything = true;
      closest_so_far = temporary_record.t;
      rec = temporary_record;
      rec.object_ID = i;
    }
  }
  for (size_t i = 0; i < nPlanes; i++) {
    if (planeHit(planes[i], r, closest_so_far, temporary_record)) {
      // std::cout << "an object was hit\n";
      hit_anything = true;
      closest_so_far = temporary_record.t;
      rec = temporary_record;
      rec.object_ID = i;
    }
  }
  for (size_t i = 0; i < nTriangles; i++) {
    if (triangleHit(triangles[i], r, closest_so_far, temporary_record)) {
      // std::cout << "an object was hit\n";
      hit_anything = true;
      closest_so_far = temporary_record.t;
      rec = temporary_record;
      rec.object_ID = i;
    }
  }
  return hit_anything;
}

__device__ bool lightHit(const ray &r, float max_val, hitRecord &current_point,
                         const triangle *triangles, const sphere *spheres,
                         const plane *planes, int nSpheres, int nPlanes,
                         int nTriangles) {
  hitRecord temporary_record;

  for (size_t i = 0; i < nSpheres; i++) {
    if (sphereHit(spheres[i], r, max_val, temporary_record) &&
        i != current_point.object_ID) {
      return true;
    }
  }
  for (size_t i = 0; i < nPlanes; i++) {
    if (planeHit(planes[i], r, max_val, temporary_record) &&
        i != current_point.object_ID) {
      return true;
    }
  }
  for (size_t i = 0; i < nTriangles; i++) {
    if (triangleHit(triangles[i], r, max_val, temporary_record) &&
        i != current_point.object_ID) {
      return true;
    }
  }
  return false;
}

__device__ ray generateRay(struct pointSource s, glm::vec3 point) {
  // no need to normalize because you wont need the distance from the origin to
  // the intersect for lighting intersections.
  ray r;
  r.dir = glm::normalize(s.pos - point);
  r.orig = point;
  return r;
}

__device__ glm::vec3
onHitShadow(const sphere *sp, const triangle *tr, const plane *pl,
            hitRecord &rec, float intersect_distance_maximum,
            int number_of_lights, const struct pointSource *lights,
            const int nSpheres, const int nTriangles, const int nPlanes) {
  glm::vec3 color_intensity{0.0, 0.0, 0.0};
  hitRecord temp_record;
  float intensity;
  glm::vec3 color;
  ray lighting_ray;

  for (int i = 0; i < number_of_lights; i++) {
    lighting_ray = generateRay(lights[i], rec.p);
    // std::cout << "LIGHT VECTOR: x " << lighting_ray.direction().x << ", y "
    // << lighting_ray.direction().y << ", z " << lighting_ray.direction().z <<
    // "\n"; std::cout << "NORMAL VECTOR: x " << rec.normal.x << ", y " <<
    // rec.normal.y << ", z " << rec.normal.z << "\n";
    intensity =
        std::max(glm::dot(lighting_ray.dir, rec.normal), static_cast<float>(0));
    // std::cout << "intensity: " << intensity << "\n";
    if (intensity > static_cast<float>(0)) {
      if (!lightHit(lighting_ray, intersect_distance_maximum, rec, tr, sp, pl,
                    nSpheres, nPlanes, nTriangles)) {
        color_intensity.r += intensity * lights[i].col.r;
        color_intensity.g += intensity * lights[i].col.g;
        color_intensity.b += intensity * lights[i].col.b;
      }
    }
  }
  color = {std::clamp(color_intensity.r, 0.0f, 1.0f) * rec.color.r,
           std::clamp(color_intensity.g, 0.0f, 1.0f) * rec.color.g,
           std::clamp(color_intensity.b, 0.0f, 1.0f) * rec.color.b};

  return color;
}

__device__ glm::vec3 cameraRay(const struct camera2 &cam, const float x,
                               const float y) {
  const glm::vec3 side = glm::cross(cam._view, cam._up);
  const glm::vec3 result = x * side + y * cam._up + cam._imagedist * cam._view;
  return glm::normalize(result);
}

__global__ void
kernelPerPixel(unsigned char *image_data, const struct camera2 *cam,
               const struct plane *pl, const struct triangle *tr,
               const struct sphere *sp, const struct pointSource *lights,
               const int y_res, const int x_res,
               float intersect_distance_maximum, int nSpheres, int nPlanes,
               int nTriangles, int nLights) {
  float x_pixel_position;
  float y_pixel_position;
  glm::vec3 color;
  hitRecord rec;

  int x = blockIdx.x * blockDim.x + threadIdx.x;
  int y = blockIdx.y * blockDim.y + threadIdx.y;

  if (x < x_res && y < y_res) {

    x_pixel_position = (static_cast<float>(x) / (float)y_res - 0.5f);
    y_pixel_position = (static_cast<float>(y) / (float)x_res - 0.5f);
    struct ray r;
    r.orig = cam->_position;
    r.dir = glm::normalize(cameraRay(*cam, x_pixel_position, y_pixel_position));
    if (closestHit(r, intersect_distance_maximum, rec, tr, sp, pl, nSpheres,
                   nPlanes, nTriangles)) {
      color = onHitShadow(sp, tr, pl, rec, intersect_distance_maximum, nLights,
                          lights, nSpheres, nTriangles, nPlanes);
      image_data[3 * (y * y_res + x) + 0] =
          static_cast<unsigned char>(255 * color.r);
      image_data[3 * (y * y_res + x) + 1] =
          static_cast<unsigned char>(255 * color.g);
      image_data[3 * (y * y_res + x) + 2] =
          static_cast<unsigned char>(255 * color.b);
    } else {
      image_data[3 * (y * y_res + x) + 0] = 0;
      image_data[3 * (y * y_res + x) + 1] = 0;
      image_data[3 * (y * y_res + x) + 2] = 0;
    }
  }
}
