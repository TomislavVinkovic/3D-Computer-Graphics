#pragma once
#include "geometry.h"

struct Material
{  
    Vec3f diffuse_color;
    float diffuse_coef{1};
    float spec_coef{1};
    float opacity;
    float refr_coef{1};
    float phong_exp{1};
    float mirroring_intensity{0.5};
    
    Material(const Vec3f &color) : diffuse_color(color), opacity{1} {}
    Material() : diffuse_color(Vec3f(0, 0, 0)), opacity{1} {}
};