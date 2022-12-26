#pragma once

#include "tgaimage.h"
#include<cmath>
#include<algorithm>


TGAColor operator*(float c, const TGAColor& color) {
    return TGAColor(c * color.r, c * color.g, c * color.b, c * color.a);
}

TGAColor operator+(const TGAColor& c1, const TGAColor& c2) {
    float a0 = static_cast<float>(c1.a) / 255;
    float r0 = static_cast<float>(c1.r) / 255;
    float g0 = static_cast<float>(c1.g) / 255;
    float b0 = static_cast<float>(c1.b) / 255;

    float a1 = static_cast<float>(c2.a) / 255;
    float r1 = static_cast<float>(c2.r) / 255;
    float g1 = static_cast<float>(c2.g) / 255;
    float b1 = static_cast<float>(c2.b) / 255;

    float a01 = (1.0f-a0) * a1 + a0;
    return TGAColor(
        static_cast<unsigned char> (floor( ((1.0f - a0) * a1*r1 + a0 * r0) / a01 * 255) ),
        static_cast<unsigned char> (floor( ((1.0f - a0) * a1*g1 + a0 * g0) / a01 * 255)),
        static_cast<unsigned char> (floor( ((1.0f - a0) * a1*b1 + a0 * b0) / a01 * 255) ),
        static_cast<unsigned char> (floor(a01 * 255))
    );
}