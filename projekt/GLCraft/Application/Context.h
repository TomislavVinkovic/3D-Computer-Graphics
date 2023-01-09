#pragma once

#include "glad/glad.h"
#include<GLFW/glfw3.h>

class Application {
    private:
        unsigned int scr_width;
        unsigned int scr_height;
        float FOV = 45;

        GLFWwindow* window;

        float deltaTime = 0.f;
        float lastFrame = 0.f;

        void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void processInput(GLFWwindow *window);
        void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void mouse_callback(GLFWwindow* window, double xPos, double yPos);
        void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
};
