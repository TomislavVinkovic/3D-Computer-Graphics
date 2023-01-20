#pragma once
#include "glad/glad.h"
#include<GLFW/glfw3.h>
#include "glm.hpp"

#include "Shader.hpp"
#include "Camera.h"
#include "Block/Block.h"
#include "Block/Light.h"
#include "AbstractObjectRenderer.h"

#include <vector>
#include <string>

class CubeRenderer : public AbstractObjectRenderer{
    public:
        CubeRenderer(
                const std::string& vertexShaderPath,
                const std::string& fragmentShaderPath,
                const std::string& lightVertexShaderPath,
                const std::string& lightFragmentShaderPath
        );

        //void operator=(const CubeRenderer&);

        //void add(const glm::vec3& pos);
        void render(Camera& camera);

//        void setShader();
//        void setTexture(const std::string& filePath);

        void deleteData();
    private:
        Shader shader;
        Shader lightShader;

        std::vector<glm::vec3> pos;
        Block block;
        Light light;

        glm::vec3 blockPos;
        glm::vec3 lightPos;
};
