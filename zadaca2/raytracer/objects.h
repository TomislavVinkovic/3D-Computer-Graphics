#pragma once
#include <cmath>
#include<algorithm>
#include "geometry.h"
#include "material.h"
#include "ray.h"
#include <iostream>
using namespace std;

struct Object
{
    Material material;
    
    virtual bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const = 0;    
};

struct Sphere : Object
{
    Vec3f c; // centar
    float r; // radius
    
    Sphere(const Vec3f &c, const float &r, const Material &mat) : c(c), r(r)
    {
        Object::material = mat;
    }
    
    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const
    {
        float d2 = ray.direction * ray.direction;
        Vec3f e_minus_c = ray.origin - c;
        
        float disc = pow(ray.direction * e_minus_c, 2) - d2 * (e_minus_c * e_minus_c - r * r);
        
        bool ray_inside_sphere = e_minus_c * e_minus_c <= r * r;
        
        if (disc < 0)
        {
            return false;            
        }
        else
        {
            if (ray_inside_sphere)
            {
                t = (-ray.direction * e_minus_c + sqrt(disc))/d2;
            }
            else
            {
                t = (-ray.direction * e_minus_c - sqrt(disc))/d2;                
            }
            
            Vec3f normal_origin = ray.origin + ray.direction * t;
            normal = (normal_origin - c).normalize();
            
            return t > 0;
        }
    }
};

struct Cuboid : public Object {
    Vec3f min;
    Vec3f max;
    Cuboid(const Vec3f& min, const Vec3f& max, const Material& material) 
        : min{min}, max{max} {
            this->material = material;
        }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const {

        float tn = numeric_limits<float>::min();
        float tf = numeric_limits<float>::max();

        float minX = std::min(min.x, max.x);
        float minY = std::min(min.y, max.y);
        float minZ = std::min(min.z, max.z);
        float maxX = std::max(min.x, max.x);
        float maxY = std::max(min.y, max.y);
        float maxZ = std::max(min.z, max.z);

        Vec3f p = ray.origin;
        Vec3f d = ray.direction;

        //osigurac za potencijalno dijeljenje s 0
        if(d.x == 0) {
            //cout << "dx je 0" << endl;
            if(p.x < minX || p.x > maxX) return false;
        }

        else {
            float txmin = (minX - p.x)/d.x;
            float txmax = (maxX - p.x)/d.x;
            if(txmin > txmax) swap(txmin, txmax);

            tn = std::max(tn, txmin);
            tf = std::min(tf, txmax);
            if(tn > tf || tf < 0) {
                //cout << "Problem kod tx" << endl;
                return false;
            }
        }
        t = tn;
        /////////////////////////////
        if(d.y == 0) {
            //cout << "dy je 0" << endl;
            if(p.y < minY || p.y > maxY) return false;
        }
        else {
            float tymin = (minY - p.y)/d.y;
            float tymax = (maxY - p.y)/d.y;
            if(tymin > tymax) swap(tymin, tymax);

            tn = std::max(tn, tymin);
            tf = std::min(tf, tymax);
            if(tn > tf || tf < 0) {
                //cout << "Problem kod ty" << endl;
                return false;
            }
        }
        t = tn;

        /////////////////////////////
        if(d.z == 0) {
            //cout << "dz je 0" << endl;
            if(p.z < minZ || p.z > maxZ) return false;
        }
        else {
            float tzmin = (minZ - p.z)/d.z;
            float tzmax = (maxZ - p.z)/d.z;
            if(tzmin > tzmax) swap(tzmin, tzmax);

            tn = std::max(tn, tzmin);
            tf = std::min(tf, tzmax);
            if(tn > tf || tf < 0) {
                //cout << "Problem kod tz" << endl;
                return false;
            }
        }
        t = tn;

        Vec3f hit_point = p + d*t;
        
       
       const float eps = 0.01;    
        if(abs(hit_point.x - min.x) < eps) {
            normal = Vec3f(-1, 0, 0);
        }
        else if(abs(hit_point.x - max.x) < eps) {
            normal = Vec3f(1, 0, 0);
        }
        else if(abs(hit_point.y - min.y) < eps) {
            normal = Vec3f(0, -1, 0);
        }
        else if(abs(hit_point.y - max.y) < eps) {
            normal = Vec3f(0, 1, 0);
        }
        else if(abs(hit_point.z - min.z) < eps) {
            normal = Vec3f(0, 0, -1);
        }
        else if(abs(hit_point.z - max.z) < eps) {
            normal = Vec3f(0, 0, 1);
        }
        
        return true;
    }
};