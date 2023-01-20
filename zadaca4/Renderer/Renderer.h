#pragma once
#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include "glm.hpp"
#include "Shader.hpp"

#include "Cube/CubeRenderer.h"

class Renderer {
    private:
        glm::vec4 clearColor;
        CubeRenderer cubeRenderer;
    public:
        Renderer(
            unsigned int scr_width,
            unsigned int scr_height,
            const std::string& cubeVertexPath,
            const std::string& cubeFragmentPath,
            const std::string& lightVertexShaderPath,
            const std::string& lightFragmentShaderPath,
            glm::vec4 clearColor = glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)
            //glm::vec4 clearColor = glm::vec4(0.f, 0.77f, 1.f, 1.0f)
        ) : cubeRenderer(CubeRenderer(cubeVertexPath, cubeFragmentPath, lightVertexShaderPath, lightFragmentShaderPath)){
            this->clearColor = clearColor;
        }

        //void drawCube(const glm::vec3& pos);
        //void drawChunk(const glm::vec3& pos);

        void setCubeRenderer(const CubeRenderer& cubeRenderer);

        void render(GLFWwindow* window, Camera& camera);

        //ciscenje podataka na kraju
        void clearData();
};
