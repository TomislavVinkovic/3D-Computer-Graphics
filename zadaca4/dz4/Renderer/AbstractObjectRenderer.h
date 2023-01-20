#pragma once

#include "Camera.h"

class AbstractObjectRenderer {
    public:
        virtual void render(Camera& camera) = 0;
        virtual void deleteData() = 0;
};
