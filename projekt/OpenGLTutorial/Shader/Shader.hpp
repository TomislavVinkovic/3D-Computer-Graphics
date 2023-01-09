#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);

        //activate the shader
        void use();

        //utility uniform methods
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;

        //utility compilation methods
        void checkCompilationErrors(unsigned int shader, int type = 0, bool program = false);
};