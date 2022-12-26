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
    vector<float>{1.f, -1.f, 0.f, 0.f},
    vector<float>{0.f, 1.f, 0.f, 0.f},
    vector<float>{-1.f, -1.f, 1.f, 0.f},
    vector<float>{0.f, -1.f, -20.f, 1.f}
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
    Vec3f p0{
        x0, 
        y0, 
        z0
    };
    Vec3f p1{
        x1, 
        y1, 
        z1
    };
    Vec3f p2{
        x2, 
        y2, 
        z2
    };

    Vec3f pCam0;
    Vec3f pCam1;
    Vec3f pCam2;

    vector<Vec3f> camVecs{pCam0, pCam1, pCam2};

    //prebacivanje u prostor kamere
    camera.multVecMatrix(p0, camVecs[0]);
    camera.multVecMatrix(p1, camVecs[1]);
    camera.multVecMatrix(p2, camVecs[2]);

    vector<Vec3f> rasterVecs;

    for (auto& e : camVecs)
    {
        //prebacivanje u prostor ekrana
        Vec2f ps;
        ps.x = nearPlane * e.x / -e.z;
        ps.y = nearPlane * e.y / -e.z;

        //preslikavanje vertexa u kanonski pogled
        Vec2f pNDC;
        pNDC.x = 2 * ps.x / (r - l) - (r + l) / (r - l);
        pNDC.y = 2 * ps.y / (t - b) - (t + b) / (t - b);

        //rasteriziran trokut
        Vec3f pRaster;
        pRaster.x = (pNDC.x + 1) / 2 * width;
        pRaster.y = (1 - pNDC.y) / 2 * height;
        pRaster.z = -e.z;

        rasterVecs.push_back(pRaster);
    }
    //CRTANJE TROKUTA
    //prebacivanje koordinata trokuta u raster space
    Vec3f& v0Raster = rasterVecs[0];
    Vec3f& v1Raster = rasterVecs[1];
    Vec3f& v2Raster = rasterVecs[2];

    v0Raster.z = 1 / v0Raster.z;
    v1Raster.z = 1 / v1Raster.z;
    v2Raster.z = 1 / v2Raster.z;

    float xmin = floor(min(x0, min(x1, x2)));
    float xmax = ceil(max(x0, max(x1, x2)));
    
    float ymin = floor(min(y0, min(y1, y2)));
    float ymax = ceil(max(y0, max(y1, y2)));

    if (xmin > width - 1 || xmax < 0 || ymin > height - 1 || ymax < 0) {
        return;
    }

    //boundamo xmin i ymin u prostor ekrana
    uint32_t xMinBound = max(0, (int)floor(xmin));
    uint32_t xMaxBound = min(int(width) - 1, (int)floor(xmax));
    uint32_t yMinBound = max(0, (int)floor(ymin));
    uint32_t yMaxBound = min(int(height) - 1, (int)floor(ymax));

    float P = fac3D(v0Raster.x, v1Raster, v2Raster);
    

    for(uint32_t y = yMinBound; y <= yMaxBound; y++) {
        for(uint32_t x = xMinBound; x <= xMaxBound; x++) {
            float p_x = x + 0.5f;
            float p_y = y + 0.5f;

            Vec3f bary_vec(p_x, p_y, 0);

            float w0 = abs(fac3D(v1Raster, v2Raster, bary_vec));
            float w1 = abs(fac3D(v2Raster, v0Raster, bary_vec));
            float w2 = abs(fac3D(v0Raster, v1Raster, bary_vec));

            //cout << w2 << endl;
            if(w0 >= 0 && w1 >= 0 && w2 >= 0) {
                
                w0 /= P;
                w1 /= P;
                w2 /= P;

                float oneOverZ = v0Raster.z * w0 + v1Raster.z * w1 + v2Raster.z * w2;
                float z = 1/t;

                if(z < zBuffer[y * width + x]) {
                    zBuffer[y * width + x] = z;
                }
                
                //prebacivanje kompletne slike u prostor kamere
                Vec3f v0Cam, v1Cam, v2Cam;
                camera.multVecMatrix(v0Raster, v0Cam);
                camera.multVecMatrix(v1Raster, v1Cam);
                camera.multVecMatrix(v2Raster, v2Cam);

                float px = (v0Cam.x/-v0Cam.z) * w0 + (v1Cam.x/-v1Cam.z) * w1 + (v2Cam.x/-v2Cam.z) * w2; 
                float py = (v0Cam.y/-v0Cam.z) * w0 + (v1Cam.y/-v1Cam.z) * w1 + (v2Cam.y/-v2Cam.z) * w2; 
                Vec3f pt(px * z, py * z, -z);  //pt je sad u prostoru kamere

                //racunanje normale
                Vec3f normal = (v1Cam - v0Cam).crossProduct(v2Cam - v0Cam);
                normal.normalize();
                Vec3f direction = -pt;
                direction.normalize();
                image.set(x, y, color);

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

    draw_triangle(image, -1, 1, -1, -1, 1, 1, 0.999999, 1, 1, blue, zBuffer);
    //draw_triangle(image, 20, 30, 0, 180, 80, 0, 100, 200, 0, blue, zBuffer);

    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("zad2.tga");

    delete[] zBuffer;
}