cmake_minimum_required(VERSION 3.24)
project(Zadaca4)

set(CMAKE_CXX_STANDARD 17)
include_directories(src include/glad include/stb_image include/glm Shader Application Camera State Renderer Model Texture Model/Chunk Player World NoiseGenerator)

set(SOURCE_FILES
        include/glad/glad.c
        main.cpp Camera/Camera.cpp
        Application/Context.cpp

        Application/Application.cpp
        Renderer/Renderer.cpp
        Renderer/Cube/CubeRenderer.cpp
        Model/Block/Block.cpp
        Model/Block/Light.cpp
)


add_executable(GLCraft ${SOURCE_FILES})

target_link_libraries(GLCraft glfw3 wayland-client m GL GLU)
