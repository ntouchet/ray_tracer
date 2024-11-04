#pragma once
#include <memory>
#include <string>
#include <glm/fwd.hpp>
#include <tira/parser.h>
#include <tira/image.h>
#include <tira/graphics/camera.h>
#include <glm/glm.hpp>
#include <thread>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>
#include <sstream>
#include "hittable.h"
#include "sphere.h"
#include "hittableList.h"
#include "light.h"
#include "timing.h"
#include "triangle.h"
#include <limits>




class rayTracer
{
public:
    rayTracer(const char* file_name)
        : m_scene_file(file_name)
    {
        setLights(m_scene_file);
        setCamera(m_scene_file);
        setHittables(m_scene_file);
        setBackground(m_scene_file);
        setImage(m_scene_file);
    }

private:
    //Multi Threading


    //Define Constants
    float m_intersect_distance_maximum = std::numeric_limits<float>::max();

    //Camera
    tira::camera m_cam;
    float m_fov;
    glm::vec3 m_camera_position;
    glm::vec3 m_camera_lookat;
    glm::vec3 m_camera_up;

    //Image
    int m_resolution[2];
    int* m_image_data;

    //Scene
    tira::parser m_scene_file;
    hittableList m_world;
    pointSource* m_lights;
    int m_number_of_lights;
    int m_number_of_spheres;
    glm::vec3 m_background_color;

    //Profiling
    timingInfo m_timing = {0,0,0,0,0,0,0};

public:
    std::string timingStr()
    {
        std::stringstream ss;
        ss<<"---Timing---\n"<< "Total Trace Time (not including loading file): " << ((double)m_timing.total_trace/(1'000.0));

        return ss.str();
    }


    void multiThreadedCpuTrace(int number_of_threads)
    {

        auto start = std::chrono::high_resolution_clock::now();
        int y_res = m_resolution[1];
        int* image_data = new int[m_resolution[0]*m_resolution[1]*3];

        tira::image<unsigned char>I(m_resolution[0],m_resolution[1],3);
        int strip_size = y_res/number_of_threads;

        std::thread* threads = new std::thread[number_of_threads];
        for(int i = 0; i<number_of_threads; i++){
            int start_index = i*strip_size;
            int end_index = (i==number_of_threads-1) ? y_res : (i+1)*strip_size;
            threads[i] = std::thread(&rayTracer::kernel, this, start_index, end_index, image_data, m_cam, m_world, m_resolution[0], m_resolution[1]);
        }

        for(int i = 0; i<number_of_threads; i++){
            threads[i].join();
        }

        delete[] threads;

        for(int y = 0; y<m_resolution[1];y++)
        {
            for(int x = 0; x<m_resolution[0];x++)
            {
                    I(x,y,0)=image_data[3*(y*m_resolution[0]+x)+0];
                    I(x,y,1)=image_data[3*(y*m_resolution[0]+x)+1];
                    I(x,y,2)=image_data[3*(y*m_resolution[0]+x)+2];
            }
        }
        I.save("output.bmp");

        delete[] image_data;

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
        m_timing.total_trace = (long long)duration;
    }

    void trace()
    {

        auto start = std::chrono::high_resolution_clock::now();

        tira::image<unsigned char> I(m_resolution[0],m_resolution[1],3);

        glm::vec3 color = {0,0,0};
        float x_pixel_position {};
        float y_pixel_position {};
        hitRecord rec;
        //std::cout << m_cam << "\n";
        for(int y = 0; y<m_resolution[1];y++)
        {
            int imag_y = (m_resolution[1]-y);
            for(int x = 0; x<m_resolution[0];x++)
            {
                x_pixel_position = (static_cast<float>(x)/(float)m_resolution[0] - 0.5f);
                y_pixel_position = (static_cast<float>(imag_y)/(float)m_resolution[1] - 0.5f);
                ray r(m_cam.position(), glm::normalize(m_cam.ray(x_pixel_position,y_pixel_position)));
                //std::cout << "(" << x << ", " << y << ")";
                //std::cout << "position: (" << r.origin().x << ", " << r.origin().y << ", " << r.origin().z << ")" << " direction: (" << r.direction().x << ", " << r.direction().y << ", " << r.direction().z << ")\n";
                if(m_world.hit(r,m_intersect_distance_maximum,rec))
                {
                    color = onHitShadow(rec);
                    //std::cout << "r " << color.r << " " << "g " << color.g << " " << "b " << color.b << "\n";

                    I(x,y,0)=static_cast<int>(255*color.r);
                    I(x,y,1)=static_cast<int>(255*color.g);
                    I(x,y,2)=static_cast<int>(255*color.b);
                }
                else
                {
                    I(x,y,0)=0;
                    I(x,y,1)=0;
                    I(x,y,2)=0;
                }

            }
        }

        I.save("output.bmp");

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime-start).count();
        m_timing.total_trace = (long long)duration;
    }

private:

    void kernel(int start_row, int end_row, int* image_data, tira::camera cam, const hittableList world, const int y_res, const int x_res)
    {

        int x_pixel_position;
        int y_pixel_position;
        glm::vec3 color;
        hitRecord rec;

        for(int y = start_row; y < end_row; y++)
        {
            int imag_y = (y_res-y);
            for(int x = 0; x<x_res;x++)
            {
                x_pixel_position = (static_cast<float>(x)/(float)y_res - 0.5f);
                y_pixel_position = (static_cast<float>(imag_y)/(float)x_res - 0.5f);
                ray r(cam.position(), glm::normalize(cam.ray(x_pixel_position,y_pixel_position)));

                if(world.hit(r,m_intersect_distance_maximum,rec))
                {
                    color = onHitShadow(rec);
                    //std::cout << rec.object_ID << " " <<glm::to_string(color) << "\n";

                    image_data[3*(y*y_res+x)+0]=static_cast<int>(255*color.r);
                    image_data[3*(y*y_res+x)+1]=static_cast<int>(255*color.g);
                    image_data[3*(y*y_res+x)+2]=static_cast<int>(255*color.b);
                }
                else
                {
                    image_data[3*(y*y_res+x)+0]=0;
                    image_data[3*(y*y_res+x)+1]=0;
                    image_data[3*(y*y_res+x)+2]=0;
                }
            }
        }
    }

    glm::vec3 onHitShadow(hitRecord& rec)
    {
        glm::vec3 color_intensity {0.0,0.0,0.0};
        hitRecord temp_record;
        float intensity;
        glm::vec3 color;
        ray lighting_ray;

        for(int i = 0; i < m_number_of_lights; i++)
        {
            lighting_ray = m_lights[i].generateRay(rec.p);
            //std::cout << "LIGHT VECTOR: x " << lighting_ray.direction().x << ", y " << lighting_ray.direction().y << ", z " << lighting_ray.direction().z << "\n";
            //std::cout << "NORMAL VECTOR: x " << rec.normal.x << ", y " << rec.normal.y << ", z " << rec.normal.z << "\n";
            intensity = std::max(glm::dot(lighting_ray.direction(),rec.normal),static_cast<float>(0));
            //std::cout << "intensity: " << intensity << "\n";
            if (intensity > static_cast<float>(0))
            {
                if(!m_world.lightHit(lighting_ray,m_intersect_distance_maximum,rec))
                {
                    color_intensity.r += intensity*m_lights[i].col.r;
                    color_intensity.g += intensity*m_lights[i].col.g;
                    color_intensity.b += intensity*m_lights[i].col.b;
                }
            }

        }
        color = {std::clamp(color_intensity.r,0.0f,1.0f)*rec.color.r, std::clamp(color_intensity.g,0.0f,1.0f)*rec.color.g, std::clamp(color_intensity.b,0.0f,1.0f)*rec.color.b};


        return color;

    }

    glm::vec3 onHit(hitRecord& rec)
    {
        glm::vec3 color_intensity {0.0,0.0,0.0};
        hitRecord temp_record;
        float intensity;
        glm::vec3 color;
        ray lighting_ray;

        for(int i = 0; i < m_number_of_lights; i++)
        {
            lighting_ray = m_lights[i].generateRay(rec.p);
            //std::cout << "LIGHT VECTOR: x " << lighting_ray.direction().x << ", y " << lighting_ray.direction().y << ", z " << lighting_ray.direction().z << "\n";
            //std::cout << "NORMAL VECTOR: x " << rec.normal.x << ", y " << rec.normal.y << ", z " << rec.normal.z << "\n";
            intensity = std::max(glm::dot(lighting_ray.direction(),rec.normal),static_cast<float>(0));
            //std::cout << "intensity: " << intensity << "\n";
            color_intensity.r += intensity*m_lights[i].col.r;
            color_intensity.g += intensity*m_lights[i].col.g;
            color_intensity.b += intensity*m_lights[i].col.b;

        }
        color = {std::clamp(color_intensity.r,0.0f,1.0f)*rec.color.r, std::clamp(color_intensity.g,0.0f,1.0f)*rec.color.g, std::clamp(color_intensity.b,0.0f,1.0f)*rec.color.b};
        return color;

    }


    //Functions To Setup rayTracer from a file
    // Gets the lights from .scene file and makes list of all lights
    void setLights(tira::parser& scene_file)
    {
        m_number_of_lights = scene_file.count("light");
        m_lights = new pointSource[m_number_of_lights];

        for(int i=0;i<m_number_of_lights;i++)
        {
            glm::vec3 position(scene_file.get<float>("light",i,0),scene_file.get<float>("light",i,1),scene_file.get<float>("light",i,2));
            glm::vec3 color(scene_file.get<float>("light",i,3),scene_file.get<float>("light",i,4),scene_file.get<float>("light",i,5));
            m_lights[i] = pointSource(position,color);
        }


    }

    // Gets the spheres from .scene file and makes list of all lights
    void setHittables(tira::parser& scene_file)
    {
        m_number_of_spheres = scene_file.count("sphere");

        for(int i=0;i<m_number_of_spheres;i++)
        {
            glm::vec3 position(scene_file.get<float>("sphere",i,1),scene_file.get<float>("sphere",i,2),scene_file.get<float>("sphere",i,3));
            glm::vec3 color(scene_file.get<float>("sphere",i,4),scene_file.get<float>("sphere",i,5),scene_file.get<float>("sphere",i,6));
            float radius = scene_file.get<float>("sphere",i,0);
            std::shared_ptr<hittable> Sphere = std::make_shared<sphere>(color, position, radius);
            m_world.add(Sphere);
        }

        int number_of_planes = scene_file.count("plane");

        for(int i=0;i<number_of_planes;i++)
        {
            glm::vec3 point(scene_file.get<float>("plane",i,0),scene_file.get<float>("plane",i,1),scene_file.get<float>("plane",i,2));
            glm::vec3 color(scene_file.get<float>("plane",i,6),scene_file.get<float>("plane",i,7),scene_file.get<float>("plane",i,8));
            glm::vec3 normal(scene_file.get<float>("plane",i,3),scene_file.get<float>("plane",i,4),scene_file.get<float>("plane",i,5));
            std::shared_ptr<hittable> Plane = std::make_shared<plane>(point, normal, color);
            m_world.add(Plane);
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
                m_world.add(Triangle);


            }


            //std::cout << m_world.objects.size() << "\n";
        }
        for(const auto& object : m_world.objects)
        {
            //std::cout << object->str();
        }

    }

//  gets camera position, up vector, lookat vector, and fov from .scene file
    void setCamera(tira::parser& scene_file)
    {
        m_camera_position = {scene_file.get<float>("camera_position",0),scene_file.get<float>("camera_position",1),scene_file.get<float>("camera_position",2)};
        m_camera_lookat = {scene_file.get<float>("camera_look",0),scene_file.get<float>("camera_look",1),scene_file.get<float>("camera_look",2)};
        m_camera_up = {scene_file.get<float>("camera_up",0),scene_file.get<float>("camera_up",1),scene_file.get<float>("camera_up",2)};
        m_fov = scene_file.get<float>("camera_fov",0);
        m_cam.position(m_camera_position);
        m_cam.lookat(m_camera_lookat);
        m_cam.up(m_camera_up);
        m_cam.fov(m_fov);

    }

    // Gets the background color of the scene
    void setBackground(tira::parser& scene_file)
    {
        m_background_color = {scene_file.get<int>("background",0),scene_file.get<int>("background",1),scene_file.get<int>("background",2)};
    }

    // Gets the resolution of the scene
    void setImage(tira::parser& scene_file)
    {
        m_resolution[0] = scene_file.get<int>("resolution",0);
        m_resolution[1] = scene_file.get<int>("resolution",1);
    }


};
