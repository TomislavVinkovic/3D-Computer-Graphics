#pragma once
#include <vector>
#include "glad/glad.h"
#include<GLFW/glfw3.h>

#include "Model.h"

class Block : public Model{
    public:
        Block();
        virtual void setIndices(std::vector<unsigned int>) override;
        virtual void setVertices(std::vector<float>) override;
};
