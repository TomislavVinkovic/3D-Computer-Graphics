#include <iostream>
#include <cmath>
#include <algorithm>
#include<limits>
#include <cstdlib>
#include "geometry.h"
#include "tgaimage.h"
#include "tgaOperator.hpp"
using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green  = TGAColor(0, 255, 0, 255);


//konstante koje trebamo za 3d transformaciju
static const float FOV = 45;
static const float nearPlane = 1;
static const float farPlane = 1000;

static float t,b,l,r;

const Matrix4x4f camera = {
    vector<float>{1.f, 0.f, 0.f, 0.f},
    vector<float>{0.f, 1.f, 0.f, 0.f},
    vector<float>{0.f, 0.f, 1.f, 0.f},
    vector<float>{0.f, 0.f, 0.f, 1.f}
};

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);    
}

//pomocna funkcija za racunanje baricentricnih koordinata
float fac(float Xa, float Ya, float Xc, float Yc, float x, float y) {
    return (Ya - Yc) * x + (Xc - Xa) * y + Xa*Yc - Xc*Ya;
}

float fac3D(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2) {
    return (v2.x - v0.x) * (v1.y - v0.y) - (v2.y - v0.y) * (v1.x - v0.x);
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

void draw_triangle_2d_gouraurd(TGAImage& image, float x0, float y0, float x1, float y1, float x2, float y2, TGAColor c0, TGAColor c1, TGAColor c2)
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
                    TGAColor c = (alpha * c0) + (beta * c1) + (gamma * c2);
                    cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << endl;
                    set_color(x, y, image, c);
                }

            }
        }
    }
}

//crtanje trokuta u 3d
void draw_triangle(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1,  float x2, float y2, float z2, TGAColor color, float* zBuffer) {
    
    //matrica kamere izrazena preko 3 vektora
    Vec3f c2{1, 0, 0};
    Vec3f c1{0, 1, 0};
    Vec3f c0{0, 0, 1};
    
    Vec3f p2{
        x0, 
        y0, 
        z0
    };
    Vec3f p1{
        x1, 
        y1, 
        z1
    };
    Vec3f p0{
        x2, 
        y2, 
        z2
    };
    
    p0.x /= p0.z;
    p0.y /= p0.z;
    
    p1.x /= p1.z;
    p1.y /= p1.z;

    p2.x /= p2.z;
    p2.y /= p2.z;

    //konverzija iz prostora ekrana u raster prostor i odmah u NDC (normalni) prostor;
    p0.x = (1 + p0.x) * 0.5 * width;
    p0.y = (1 + p0.y) * 0.5 * height;

    p1.x = (1 + p1.x) * 0.5 * width;
    p1.y = (1 + p1.y) * 0.5 * height;

    p2.x = (1 + p2.x) * 0.5 * width;
    p2.y = (1 + p2.y) * 0.5 * height;

    //
    /*
        c0[0] /= v0[2], c0[1] /= v0[2], c0[2] /= v0[2]; 
        c1[0] /= v1[2], c1[1] /= v1[2], c1[2] /= v1[2]; 
        c2[0] /= v2[2], c2[1] /= v2[2], c2[2] /= v2[2]; 
        // pre-compute 1 over z
        v0[2] = 1 / v0[2], v1[2] = 1 / v1[2], v2[2] = 1 / v2[2]; 

    */

   //transformacija kamere, trebat cu je nesto kasnije za lijepljenje teksture
   c0.x /= p0.z; c0.y /= p0.z; c0.y /= c0.z;
   c1.x /= p1.z; c1.y /= p1.z; c1.y /= c1.z;
   c2.x /= p2.z; c2.y /= p2.z; c2.y /= c2.z;

   //'one over z'
   p0.z = 1 / p0.z;
   p1.z = 1 / p1.z;
   p2.z = 1 / p2.z;

   float area = fac3D(p0, p1, p2);
   cout << area << endl;

   for (uint32_t j = 0; j < height; ++j) { 
        for (uint32_t i = 0; i < width; ++i) { 
            Vec3f p(i + 0.5, height - j + 0.5, 0); 
            float w0 = fac3D(p1, p2, p); 
            float w1 = fac3D(p2, p0, p); 
            float w2 = fac3D(p0, p1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                cout << w0 << " " << w1 << " " << w2 << endl;
                w0 /= area; 
                w1 /= area; 
                w2 /= area; 

                set_color(i, j, image, color);
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

    // draw_triangle_2d(image, 20, 30, 180, 80, 100, 200, blue);
    // draw_triangle_2d(image, 20, 200, 180, 250, 100, 370, red);
    // draw_triangle_2d_gouraurd(image, 0, 0, 0, 512, 512, 512, red, green, blue);

    //kreacija 3d slike
    float* zBuffer = new float[width * height];
    for(int i = 0; i < width*height; i++) {
        zBuffer[i] = farPlane;
    }

    //scale definiran preko fov-a
    float scale = tan(FOV / 2.f);

    //setup ravnina odrezivanja
    r = nearPlane * scale;
    t = nearPlane * scale;
    l = -r;
    b = -t;

    draw_triangle(image, -48, -10,  82, 29, -15,  44, 13,  34, 114, blue, zBuffer);
    //draw_triangle(image, 20, 30, 0, 180, 80, 0, 100, 200, 0, blue, zBuffer);

    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("zad2.tga");

    delete[] zBuffer;
}