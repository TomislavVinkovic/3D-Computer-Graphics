#pragma once

#include "Block.h"

class Light : public Block {
    public:
        void setVertices(std::vector<float>) override;
};
