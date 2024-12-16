#Specify the version being used aswell as the language
cmake_minimum_required(VERSION 3.19)

#Name your project here
project(ray_tracer LANGUAGES CXX CUDA)

enable_language(CUDA)

#set the module directory
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 20)

#find_package(OpenGL REQUIRED)
#find_package(glfw3 CONFIG REQUIRED)
find_package(CUDAToolkit REQUIRED)
find_package(X11 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(PNG REQUIRED)
#find_package(imgui CONFIG REQUIRED core glfw-binding opengl3-binding)
#find_package(GLEW REQUIRED)
find_package(TIRA REQUIRED)
#find_package(Boost REQUIRED COMPONENTS program_options)


#build the executable in the binary directory on MS Visual Studio
if ( MSVC )
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
	SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
	SET( LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
	SET( LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif ( MSVC )

#copy resources
file(COPY ${CMAKE_SOURCE_DIR}/scenes DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#set the include directories
include_directories(
			${CMAKE_CURRENT_BINARY_DIR}
			${CMAKE_CURRENT_SOURCE_DIR}
            ${CUDA_INCLUDE_DIRS}
#			${GLFW_INCLUDE_DIRS}
			${TIRA_INCLUDE_DIRS}
)

#create an executable
add_executable(ray_tracer
                main.cu
                camera.h
                kernel.h
                loadData.h
                loadData.cpp
                functions.cu
                structs.h
)

target_link_libraries(ray_tracer
#				PRIVATE glfw
                PRIVATE ${X11_LIBRARIES}
				PRIVATE glm::glm
                PRIVATE PNG::PNG
                PRIVATE CUDA::cudart
#				PRIVATE GLEW::GLEW
#				${OPENGL_LIBRARIES}
#				${CMAKE_DL_LIBS}
#				PRIVATE imgui::imgui
#				Boost::program_options
)
