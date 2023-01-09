#include <iostream>
#include <cmath>
#include <algorithm>
#include<limits>
#include <cstdlib>
#include "geometry.h"
#include "tgaimage.h"
using namespace std;

// dimenzije slike
const static int width  = 512;
const static int height = 512;

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

float fac3D(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2) {
    return (v2.x - v0.x) * (v1.y - v0.y) - (v2.y - v0.y) * (v1.x - v0.x);
}

void draw_triangle_2d(TGAImage& image, float x0, float y0, float x1, float y1, float x2, float y2, TGAColor color)
{
    float xmin = floor(min(x0, min(x1, x2)));
    float xmax = ceil(max(x0, max(x1, x2)));
    
    float ymin = floor(min(y0, min(y1, y2)));
    float ymax = ceil(max(y0, max(y1, y2)));

    xmin = floor(max(0.f, xmin));
    xmax = ceil(min(xmax, (float)width-1));

    ymin = floor(max(0.f, ymin));
    ymax = ceil(min(ymax, (float)width-1));

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

    xmin = floor(max(0.f, xmin));
    xmax = ceil(min(xmax, (float)width-1));

    ymin = floor(max(0.f, ymin));
    ymax = ceil(min(ymax, (float)width-1));

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
                    TGAColor c = TGAColor((alpha * c0.r + beta * c1.r + gamma * c2.r), (alpha * c0.g + beta * c1.g + gamma * c2.g), (alpha * c0.b + beta * c1.b + gamma * c2.b), 255);
                    //cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << endl;
                    set_color(x, y, image, c);
                }

            }
        }
    }
}

//crtanje trokuta u 3d
void draw_triangle(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1,  float x2, float y2, float z2, TGAColor color) {
    
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

   //'one over z'
   p0.z = 1 / p0.z;
   p1.z = 1 / p1.z;
   p2.z = 1 / p2.z;

   float area = fac3D(p0, p1, p2);

   for (uint32_t y = 0; y < height; y++) { 
        for (uint32_t x = 0; x < width; x++) { 
            Vec3f p(x + 0.5, height - y + 0.5, 0); 
            float w0 = fac3D(p1, p2, p); 
            float w1 = fac3D(p2, p0, p); 
            float w2 = fac3D(p0, p1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area; 
                w2 /= area; 

                set_color(x, y, image, color);
            } 
        } 
    } 

}

void draw_triangle_tex(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path)
{
    
    TGAImage texture;
    texture.read_tga_file(path);

    //teksture
    Vec2f uv2{
        u0, v0
    };

    Vec2f uv1{
        u1, v1
    };

    Vec2f uv0{
        u2, v2
    };

    //tocke trokuta
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

    float area = fac3D(p0, p1, p2);

    for (uint32_t y = 0; y < height; y++) { 
        for (uint32_t x = 0; x < width; x++) { 
            Vec3f p(x + 0.5, height - y + 0.5, 0); 
            float w0 = fac3D(p1, p2, p); 
            float w1 = fac3D(p2, p0, p); 
            float w2 = fac3D(p0, p1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area; 
                w1 /= area; 
                w2 /= area; 
                
                float u = round((w0 * uv0.x + w1 * uv1.x + w2 * uv2.x) * texture.get_width());
                float v = round((w0 * uv0.y + w1 * uv1.y + w2 * uv2.y) * texture.get_height());

                //cout << u << " " << v << endl;
                TGAColor textureColor = texture.get(u, v);
                //cout << textureColor.r << " " << textureColor.g << " " << textureColor.b << " " << textureColor.a << endl;

                set_color(x, y, image, textureColor);
            } 
        } 
    }
}

void draw_triangle_tex_corrected(TGAImage& image, float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, const char* path) {
    TGAImage texture;
    texture.read_tga_file(path);
    
    //teksture
    Vec2f uv2{
        u0, v0
    };

    Vec2f uv1{
        u1, v1
    };

    Vec2f uv0{
        u2, v2
    };

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

    //perspektivni fix, prvi dio
    c0.x /= p0.z; c0.y /= p0.z; c0.y /= c0.z;
    c1.x /= p1.z; c1.y /= p1.z; c1.y /= c1.z;
    c2.x /= p2.z; c2.y /= p2.z; c2.y /= c2.z;

    uv0.x /= p0.z; uv0.y /= p0.z;
    uv1.x /= p1.z; uv1.y /= p1.z;
    uv2.x /= p2.z; uv2.y /= p2.z;

    //'one over z'
    p0.z = 1 / p0.z;
    p1.z = 1 / p1.z;
    p2.z = 1 / p2.z;

    float area = fac3D(p0, p1, p2);

    for (uint32_t y = 0; y < height; y++) { 
        for (uint32_t x = 0; x < width; x++) { 
            Vec3f p(x + 0.5, height - y + 0.5, 0); 
            float w0 = fac3D(p1, p2, p); 
            float w1 = fac3D(p2, p0, p); 
            float w2 = fac3D(p0, p1, p);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area; 
                w1 /= area; 
                w2 /= area; 
                
                float u = (w0 * uv0.x + w1 * uv1.x + w2 * uv2.x);
                float v = (w0 * uv0.y + w1 * uv1.y + w2 * uv2.y);

                //perspektivni fix, drugi dio
                float z = 1 / (w0 * p0[2] + w1 * p1[2] + w2 * p2[2]); 
                u *= z, v *= z; 

                u = round(u * texture.get_width());
                v = round(v * texture.get_height());

                TGAColor textureColor = texture.get(u, v);

                set_color(x, y, image, textureColor);
            } 
        } 
    }

}

int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);

    draw_triangle_2d(image, 20, 30, 180, 80, 100, 200, blue);
    draw_triangle_2d(image, 20, 200, 180, 250, 100, 370, red);
    draw_triangle_2d_gouraurd(image, 512, 512, 512, 256, 256, 256, red, green, blue);

    //kreacija 3d slike

    //draw_triangle(image, -48, -10,  82, 29, -15,  44, 13,  34, 114, blue);
    //draw_triangle_tex(image, -48, -10,  82, 29, -15,  44, 13,  34, 114, 0, 0, 1, 0, 0, 1, "./textures/cro.tga");
    //draw_triangle_tex_corrected(image, -48, -10,  82, 29, -15,  44, 13,  34, 114, 0, 0, 1, 0, 0, 1, "./textures/cro.tga");

    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("zad1_2.tga");

    return 0;
}
