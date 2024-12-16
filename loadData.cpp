
#include "loadData.h"
#include "structs.h"
#include "camera.h"
#include <tira/parser.h>
#include <tira/graphics/camera.h>
#include <memory>

void setLights(tira::parser& scene_file, pointSource* lights, const int nLights)
{
    for(int i=0;i<nLights;i++)
    {
        glm::vec3 position(scene_file.get<float>("light",i,0),scene_file.get<float>("light",i,1),scene_file.get<float>("light",i,2));
        glm::vec3 color(scene_file.get<float>("light",i,3),scene_file.get<float>("light",i,4),scene_file.get<float>("light",i,5));
        pointSource p;
        p.col = color;
        p.pos = position;
        lights[i] = p;
    }
};


void loadSpheres(tira::parser& scene_file, sphere* spheres, const int nSpheres){
    for(int i=0;i<nSpheres;i++)
    {
        glm::vec3 position(scene_file.get<float>("sphere",i,1),scene_file.get<float>("sphere",i,2),scene_file.get<float>("sphere",i,3));
        glm::vec3 color(scene_file.get<float>("sphere",i,4),scene_file.get<float>("sphere",i,5),scene_file.get<float>("sphere",i,6));
        float radius = scene_file.get<float>("sphere",i,0);
        spheres[i].m_color = color;
        spheres[i].m_radius = radius;
        spheres[i].m_center = position;
    }
};

triangle setTriangle(glm::vec3 V0, glm::vec3 V1, glm::vec3 V2) {
    triangle t;
    t.m_v[0] = V0;
    t.m_v[1] = V1;
    t.m_v[2] = V2;

    // Calculate Normal
    t.m_v0v1 = t.m_v[1] - t.m_v[0];
    t.m_v1v2 = t.m_v[2] - t.m_v[1];
    t.m_v2v0 = t.m_v[0] - t.m_v[2];
    glm::vec3 n = glm::cross(t.m_v0v1, t.m_v1v2);
    t.m_normal = glm::normalize(n);
    t.m_color = {1, 1, 1};

  // Calculate bounding sphere parameters
    t.m_centroid = (t.m_v[0] + t.m_v[1] + t.m_v[2]) / 3.0f;

    for (int i = 0; i < 3; i++) {
        float r = glm::length(t.m_centroid - t.m_v[i]);
        if (r > t.m_radius)
        t.m_radius = r;
    }

    return t;
}

void loadMesh(tira::parser& mesh_file, triangle* triangles, const int nTriangles){
        std::vector< std::vector<unsigned int> > faces;
        std::vector< std::vector<float> > vertices;
        vertices = mesh_file.get<float>("v");
        //std::cout << vertices.size() << "\n";
        faces = mesh_file.get<unsigned int>("f");


        for (size_t i = 0; i<nTriangles ; i++)
        {
            size_t v0_index = faces[i][0] - 1;
            size_t v1_index = faces[i][1] - 1;
            size_t v2_index = faces[i][2] - 1;

            glm::vec3 v0 = {vertices[v0_index][0],vertices[v0_index][1],vertices[v0_index][2]};
            glm::vec3 v1 = {vertices[v1_index][0],vertices[v1_index][1],vertices[v1_index][2]};
            glm::vec3 v2 = {vertices[v2_index][0],vertices[v2_index][1],vertices[v2_index][2]};

            triangles[i] = setTriangle(v0, v1, v2);
        }
};

void loadPlanes(tira::parser& scene_file, plane* planes, const int nPlanes){

    for(int i=0;i<nPlanes;i++)
    {
        glm::vec3 point(scene_file.get<float>("plane",i,0),scene_file.get<float>("plane",i,1),scene_file.get<float>("plane",i,2));
        glm::vec3 color(scene_file.get<float>("plane",i,6),scene_file.get<float>("plane",i,7),scene_file.get<float>("plane",i,8));
        glm::vec3 normal(scene_file.get<float>("plane",i,3),scene_file.get<float>("plane",i,4),scene_file.get<float>("plane",i,5));
        planes[i].m_color = color;
        planes[i].m_point = point;
        planes[i].m_normal = normal;
    }
}

//  gets camera position, up vector, lookat vector, and fov from .scene file
camera setCamera(tira::parser& scene_file)
{
    camera cam;
    glm::vec3 camera_position = {scene_file.get<float>("camera_position",0),scene_file.get<float>("camera_position",1),scene_file.get<float>("camera_position",2)};
    glm::vec3 camera_lookat = {scene_file.get<float>("camera_look",0),scene_file.get<float>("camera_look",1),scene_file.get<float>("camera_look",2)};
    glm::vec3 camera_up = {scene_file.get<float>("camera_up",0),scene_file.get<float>("camera_up",1),scene_file.get<float>("camera_up",2)};
    float fov = scene_file.get<float>("camera_fov",0);
    cam.position(camera_position);
    cam.lookat(camera_lookat);
    cam.up(camera_up);
    cam.fov(fov);
    return cam;

}

    // Gets the background color of the scene
glm::vec3 setBackground(tira::parser& scene_file)
{
    glm::vec3 background_color = {scene_file.get<int>("background",0),scene_file.get<int>("background",1),scene_file.get<int>("background",2)};
    return background_color;
}

    // Gets the resolution of the scene
int* setImage(tira::parser& scene_file)
{
    int* resolution = (int*)malloc(2*sizeof(int));
    resolution[0] = scene_file.get<int>("resolution",0);
    resolution[1] = scene_file.get<int>("resolution",1);
    return resolution;
}



