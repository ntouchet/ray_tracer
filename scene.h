#pragma once

#include <tira/image.h>
#include <tira/parser.h>
#include <tira/graphics/camera.h>
#include <glm/vec3.hpp>


//Sphere class
struct sphere
{
    glm::vec3 position {};
    float radius {};
    float color[3] = {0.0,0.0,0.0};
};

// Light class
struct light
{
    glm::vec3 position {};
    float color[3] = {0,0,0};
};


//Scene class that holds the objects, camera, and any other relevant information to the scene
class Scene
{
public:
    int number_of_spheres;
    int number_of_lights;
    sphere* spheres;
    light* lights;
    int resolution[2];
    float background_color[3];
    tira::camera Camera;

    Scene(const char* file_location)
    {
        tira::parser scene_file(file_location);

        setLights(scene_file);
        setSpheres(scene_file);
        setCamera(scene_file);
        setResolution(scene_file);
        setBackground(scene_file);
    }

    ~Scene()
    {
        delete[] spheres;
        delete[] lights;
    }

private:
    // Gets the lights from .scene file and makes list of all lights
    void setLights(tira::parser& scene_file)
    {
        number_of_lights = scene_file.count("light");
        lights = new light[number_of_lights];

        for(int i=0;i<number_of_lights;i++)
        {
            glm::vec3 pos(scene_file.get<float>("light",i,0),scene_file.get<float>("light",i,1),scene_file.get<float>("light",i,2));
            lights[i].position = pos;


            lights[i].color[0] = scene_file.get<float>("light",i,3);
            lights[i].color[1] = scene_file.get<float>("light",i,4);
            lights[i].color[2] = scene_file.get<float>("light",i,5);
        }


    }

    // Gets the spheres from .scene file and makes list of all lights
    void setSpheres(tira::parser& scene_file)
    {
        number_of_spheres = scene_file.count("sphere");
        spheres= new sphere[number_of_spheres];

        for(int i=0;i<number_of_spheres;i++)
        {
            glm::vec3 pos(scene_file.get<float>("sphere",i,1),scene_file.get<float>("sphere",i,2),scene_file.get<float>("sphere",i,3));
            spheres[i].position = pos;

            spheres[i].radius = scene_file.get<float>("sphere",i,0);

            spheres[i].color[0] = scene_file.get<float>("sphere",i,4);
            spheres[i].color[1] = scene_file.get<float>("sphere",i,5);
            spheres[i].color[2] = scene_file.get<float>("sphere",i,6);
        }


    }

//  gets camera position, up vector, lookat vector, and fov from .scene file
    void setCamera(tira::parser& scene_file)
    {
        glm::vec3 camera_position(scene_file.get<float>("camera_position",0),scene_file.get<float>("camera_position",1),scene_file.get<float>("camera_position",2));
        glm::vec3 camera_lookat(scene_file.get<float>("camera_look",0),scene_file.get<float>("camera_look",1),scene_file.get<float>("camera_look",2));
        glm::vec3 camera_up_vector(scene_file.get<float>("camera_up",0),scene_file.get<float>("camera_up",1),scene_file.get<float>("camera_up",2));
        Camera.lookat(camera_lookat);
        Camera.up(camera_up_vector);
        Camera.position(camera_position);
        Camera.fov(scene_file.get<float>("camera_fov",0));

    }

    // Gets the background color of the scene
    void setBackground(tira::parser& scene_file)
    {
        background_color[0] = scene_file.get<int>("background",0);
        background_color[1] = scene_file.get<int>("background",1);
        background_color[2] = scene_file.get<int>("background",2);
    }

    // Gets the resolution of the scene
    void setResolution(tira::parser& scene_file)
    {
        resolution[0] = scene_file.get<int>("resolution",0);
        resolution[1] = scene_file.get<int>("resolution",1);
    }

};
