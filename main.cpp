#include <glm/ext/vector_float3.hpp>
#include <tira/image.h>
#include <glm/vec3.hpp>
#include <cmath>
#include <tira/parser.h>
#include <tira/graphics/camera.h>
#include <iostream>
#include <algorithm>

// TODO: please put these all in their own files when you get the chance

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

// Class that represents a ray
struct ray
{
    glm::vec3 origin;
    glm::vec3 vector;
};

// This class holds relevant information about an intersect point
// normal vector (for lighting), intersect point, weather or not there was an intersection
// and the intensity of the RGB lighting at that point.
struct intersect
{
    bool intersect_state;
    glm::vec3 intersect_point = {0,0,0};
    glm::vec3 intersect_point_normal = {0,0,0};
    float intensity[3] = {0,0,0};

};

// This function detects if there was an intersect with a sphere and returns an instance of the 
// intersect struct
intersect sphereIntersect(sphere test_sphere, ray test_ray)
{
    intersect final_intersect {false,{0,0,0},{0,0,0}};

    //Test if the camera is inside the sphere
    glm::vec3 distance_to_sphere = test_sphere.position - test_ray.origin;
    float distance_to_sphere_squared = glm::dot(distance_to_sphere,distance_to_sphere);
    float radius_squared = test_sphere.radius*test_sphere.radius;
//    if(distance_to_sphere_squared < radius_squared)
//        return final_intersect;

    //Test if the sphere is behind the camera
    float distance_projected_onto_ray = glm::dot(distance_to_sphere,test_ray.vector);
    if(distance_projected_onto_ray<0)
        return final_intersect;

    //Test is the ray intersects the sphere
    float m_squared = distance_to_sphere_squared - distance_projected_onto_ray*distance_projected_onto_ray;
    if(m_squared>radius_squared)
        return final_intersect;

    //Return the position of the intersect and the normal of the sphere at that point
    final_intersect.intersect_state = true;
    float q_squared = radius_squared - m_squared;
    final_intersect.intersect_point = test_ray.origin + (distance_projected_onto_ray - std::sqrt(q_squared))*test_ray.vector;
    final_intersect.intersect_point_normal = glm::normalize(final_intersect.intersect_point - test_sphere.position);

    return final_intersect;

}

// This function assigns a value to the intensity member of the intersect struct.
// It takes the intersect and a list of all lights.
void calculateColor(intersect& p, light* lights, int number_of_lights)
{
    float intensity {};
    float intensity_r {};
    float intensity_g {};
    float intensity_b {};
    intersect light_line;

    for(int i = 0; i<number_of_lights; i++)
    {
        light_line = sphereIntersect(, )
        glm::vec3 r = glm::normalize(lights[i].position - p.intersect_point);
        intensity = std::max(glm::dot(r,p.intersect_point_normal),static_cast<float>(0));

        intensity_r += intensity*lights[i].color[0];
        intensity_g += intensity*lights[i].color[1];
        intensity_b += intensity*lights[i].color[2];
    }
    p.intensity[0] = std::clamp(intensity_r,0.0f,1.0f);
    p.intensity[1] = std::clamp(intensity_g,0.0f,1.0f);
    p.intensity[2] = std::clamp(intensity_b,0.0f,1.0f);

//    std::cout << "p in function: " << p.color[0] << "\n";
//    p.color[1] = std::min(intensity_g,static_cast<float>(1))*color[1];
//    p.color[2] = std::min(intensity_b,static_cast<float>(1))*color[2];
}

int main()
{
    Scene basic_scene("./scenes/basic.scene");
    tira::image<unsigned char> I(1000,1000,3);


    int red;
    int green;
    int blue;
    ray r;
    r.origin = basic_scene.Camera.position();
    intersect p;
    float x_pixel_position;
    float y_pixel_position;
    for(int y = 0; y<1000;y++)
    {
        for(int x = 0; x<1000;x++)
        {
            x_pixel_position = (static_cast<float>(x)/1000 - 0.5f);
            y_pixel_position = (static_cast<float>(y)/1000 - 0.5f);
            r.vector = basic_scene.Camera.ray(x_pixel_position,y_pixel_position);
            for(int i = 0; i < basic_scene.number_of_spheres; i++)
            {
                p = sphereIntersect(basic_scene.spheres[i],r);
                if(p.intersect_state)
                {
                    calculateColor(p, basic_scene.lights, basic_scene.number_of_lights);
                    red = static_cast<int>(255*p.intensity[0]*basic_scene.spheres[i].color[0]);
                    green = static_cast<int>(255*p.intensity[1]*basic_scene.spheres[i].color[1]);
                    blue = static_cast<int>(255*p.intensity[2]*basic_scene.spheres[i].color[2]);

//                   NOTE: why do I use x and y here but x and y _pixel_position in the else statement, but it works.
                    I(x,y,0)=red;
                    I(x,y,1)=green;
                    I(x,y,2)=blue;
                }
                else
                {
                    I(x_pixel_position,y_pixel_position,0)=0;
                    I(x_pixel_position,y_pixel_position,1)=0;
                    I(x_pixel_position,y_pixel_position,2)=0;
                }
            }
        }
    }

    I.save("output.bmp");

    return 0;
}
