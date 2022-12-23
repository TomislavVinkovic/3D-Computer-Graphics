#include <iostream>
#include <cmath>
#include <algorithm>
#include "tgaimage.h"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green  = TGAColor(0, 255, 0, 255);

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);    
}

//pomocna funkcija za racunanje baricentricnih koordinata
float fac(float Xa, float Ya, float Xc, float Yc, float x, float y) {
    return (Ya - Yc) * x + (Xc - Xa) * y + Xa*Yc - Xc*Ya;
}

void draw_triangle_2d(TGAImage& image, float x0, float y0, float x1, float y1, float x2, float y2, TGAColor color)
{
    float xmin = floor(min(x0, min(x1, x2)));
    float xmax = ceil(max(x0, max(x1, x2)));
    
    float ymin = floor(min(y0, min(y1, y2)));
    float ymax = ceil(max(y0, max(y1, y2)));

    //kontrolna tocka van ekrrana
    float qX = -1, qY = -1;

    for(float y = ymin; y <= ymax; y++) {
        for(float x = xmin; x <= xmax; x++) {


            float f1200 = fac(x1, y1, x2, y2, x0, y0);
            float f2011 = fac(x2, y2, x0, y0, x1, y1);
            float f0122 = fac(x0, y0, x1, y1, x2, y2);

            float alpha = fac(x1, y1, x2, y2, x, y) / f1200;
            float beta = fac(x2, y2, x0, y0, x, y) / f2011;
            float gamma = fac(x0, y0, x1, y1, x, y) / f0122;

            if(
                alpha >= 0 && alpha <= 1 && 
                beta >= 0 && beta <= 1 && 
                gamma >= 0 && gamma <= 1
            ) {
                float f12q = fac(x1, y1, x2, y2, qX, qY);
                float f20q = fac(x2, y2, x0, y0, qX, qY);
                float f01q = fac(x0, y0, x1, y1, qX, qY);

                if(
                    (alpha > 0 || f1200 * f12q >= 0) &&
                    (beta > 0 || f2011 * f20q >= 0) &&
                    (gamma > 0 || f0122 * f01q >= 0)
                ) {
                    set_color(x, y, image, color);
                }

            }
        }
    }
}

void draw_triangle_2d_gouraurd(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, TGAColor color)
{
    float xmin = floor(min(x0, min(x1, x2)));
    float xmax = ceil(max(x0, max(x1, x2)));
    
    float ymin = floor(min(y0, min(y1, y2)));
    float ymax = ceil(max(y0, max(y1, y2)));

    //kontrolna tocka van ekrrana
    float qX = -1, qY = -1;

    for(float y = ymin; y <= ymax; y++) {
        for(float x = xmin; x <= xmax; x++) {


            float f1200 = fac(x1, y1, x2, y2, x0, y0);
            float f2011 = fac(x2, y2, x0, y0, x1, y1);
            float f0122 = fac(x0, y0, x1, y1, x2, y2);

            float alpha = fac(x1, y1, x2, y2, x, y) / f1200;
            float beta = fac(x2, y2, x0, y0, x, y) / f2011;
            float gamma = fac(x0, y0, x1, y1, x, y) / f0122;

            if(
                alpha >= 0 && alpha <= 1 && 
                beta >= 0 && beta <= 1 && 
                gamma >= 0 && gamma <= 1
            ) {
                float f12q = fac(x1, y1, x2, y2, qX, qY);
                float f20q = fac(x2, y2, x0, y0, qX, qY);
                float f01q = fac(x0, y0, x1, y1, qX, qY);

                if(
                    (alpha > 0 || f1200 * f12q >= 0) &&
                    (beta > 0 || f2011 * f20q >= 0) &&
                    (gamma > 0 || f0122 * f01q >= 0)
                ) {
                    set_color(x, y, image, color);
                }

            }
        }
    }
}

void draw_triangle_tex(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path)
{
    
}

void draw_triangle_tex_corrected(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path)
{
    
}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    draw_triangle_2d(image, 20, 30, 180, 80, 100, 200, blue);
    draw_triangle_2d(image, 20, 200, 180, 250, 100, 370, red);

    //draw_triangle_2d_gouraurd(image, 20, 30, 180, 80, 100, 200, white);

    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("zad1_2.tga");
}