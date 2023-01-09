cmake_minimum_required(VERSION 3.24)
project(GLCraft)

set(CMAKE_CXX_STANDARD 17)
include_directories(src include/glad include/stb_image include/glm Shader)

set(SOURCE_FILES
        include/glad/glad.c
        main.cpp)


add_executable(GLCraft ${SOURCE_FILES})

target_link_libraries(GLCraft glfw3 wayland-client m GL GLU)
