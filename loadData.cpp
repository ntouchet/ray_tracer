
#include "loadData.h"
#include "light.h"
#include <tira/parser.h>
#include "hittableList.h"
#include "hittable.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include <memory>

lighting setLights(tira::parser& scene_file)
{
    lighting l;
    l.number_of_lights = scene_file.count("light");
    l.lights = new pointSource[l.number_of_lights];

    for(int i=0;i<l.number_of_lights;i++)
    {
        glm::vec3 position(scene_file.get<float>("light",i,0),scene_file.get<float>("light",i,1),scene_file.get<float>("light",i,2));
        glm::vec3 color(scene_file.get<float>("light",i,3),scene_file.get<float>("light",i,4),scene_file.get<float>("light",i,5));
        l.lights[i] = pointSource(position,color);
    }
    return l;
}

    // Gets the spheres from .scene file and makes list of all lights
hittableList setHittables(tira::parser& scene_file)
{
    int number_of_spheres = scene_file.count("sphere");
    hittableList world;

    for(int i=0;i<number_of_spheres;i++)
    {
        glm::vec3 position(scene_file.get<float>("sphere",i,1),scene_file.get<float>("sphere",i,2),scene_file.get<float>("sphere",i,3));
        glm::vec3 color(scene_file.get<float>("sphere",i,4),scene_file.get<float>("sphere",i,5),scene_file.get<float>("sphere",i,6));
        float radius = scene_file.get<float>("sphere",i,0);
        std::shared_ptr<hittable> Sphere = std::make_shared<sphere>(color, position, radius);
        world.add(Sphere);
    }

    int number_of_planes = scene_file.count("plane");

    for(int i=0;i<number_of_planes;i++)
    {
        glm::vec3 point(scene_file.get<float>("plane",i,0),scene_file.get<float>("plane",i,1),scene_file.get<float>("plane",i,2));
        glm::vec3 color(scene_file.get<float>("plane",i,6),scene_file.get<float>("plane",i,7),scene_file.get<float>("plane",i,8));
        glm::vec3 normal(scene_file.get<float>("plane",i,3),scene_file.get<float>("plane",i,4),scene_file.get<float>("plane",i,5));
        std::shared_ptr<hittable> Plane = std::make_shared<plane>(point, normal, color);
        world.add(Plane);
    }

    int number_of_meshes = scene_file.count("mesh");

    if (number_of_meshes > 0)
    {
        tira::parser mesh("./scenes/"+scene_file.get<std::string>("mesh",0));
        std::vector< std::vector<unsigned int> > faces;
        std::vector< std::vector<float> > vertices;
        vertices = mesh.get<float>("v");
        //std::cout << vertices.size() << "\n";
        faces = mesh.get<unsigned int>("f");
        size_t f = faces.size();


        for (size_t i = 0; i<f ; i++)
        {
            size_t v0_index = faces[i][0] - 1;
            size_t v1_index = faces[i][1] - 1;
            size_t v2_index = faces[i][2] - 1;

            glm::vec3 v0 = {vertices[v0_index][0],vertices[v0_index][1],vertices[v0_index][2]};
            glm::vec3 v1 = {vertices[v1_index][0],vertices[v1_index][1],vertices[v1_index][2]};
            glm::vec3 v2 = {vertices[v2_index][0],vertices[v2_index][1],vertices[v2_index][2]};

            std::shared_ptr<hittable> Triangle = std::make_shared<triangle>(v0,v1,v2);
            world.add(Triangle);


        }


            //std::cout << m_world.objects.size() << "\n";
    }
    return world;

}

//  gets camera position, up vector, lookat vector, and fov from .scene file
tira::camera setCamera(tira::parser& scene_file)
{
    tira::camera cam;
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



