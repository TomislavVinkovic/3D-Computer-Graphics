cmake_minimum_required(VERSION 3.24)
project(OpenGLTutorial)

set(CMAKE_CXX_STANDARD 17)
include_directories(src include/glad)

set(SOURCE_FILES
        include/glad/glad.c
        main.cpp Shader.cpp Shader.h)


add_executable(OpenGLTutorial ${SOURCE_FILES} Shader.cpp Shader.h)

target_link_libraries(OpenGLTutorial glfw3 wayland-client m GL GLU)
