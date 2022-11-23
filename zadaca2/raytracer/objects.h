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

    private:
        bool getIntersection( float fDst1, float fDst2, const Vec3f& P1, const Vec3f& P2, Vec3f &Hit) const {
            if ( (fDst1 * fDst2) >= 0.0f) return 0;
            if ( fDst1 == fDst2) return 0; 
            Hit = P1 + (P2-P1) * ( -fDst1/(fDst2-fDst1) );
            return 1;
        }
        bool inBox(Vec3f& Hit, Vec3f& B1, Vec3f& B2, int Axis) const {
            if ( Axis==1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
            if ( Axis==2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
            if ( Axis==3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
            return 0;
        }

    public:
        Vec3f min;
        Vec3f max;
        float volume;
        Material material;
        Cuboid(const Vec3f& min, const Vec3f& max, const Material& material) 
            : min{min}, max{max}, material{material} {
                float d = (max - min).norm();
                volume = d*d*d*sqrt(3)/9; //volumen kuboida
            }

    bool ray_intersect(const Ray &ray, float &t, Vec3f &normal) const {

        float tn = numeric_limits<float>::min();
        float tf = numeric_limits<float>::min();

        float minX = std::min(min.x, max.x);
        float minY = std::min(min.y, max.y);
        float minZ = std::min(min.z, max.z);
        float maxX = std::max(min.x, max.x);
        float maxY = std::max(min.y, max.y);
        float maxZ = std::max(min.z, max.z);

        auto p = ray.origin;
        auto d = ray.direction;

        //osigurac za potencijalno dijeljenje s 0
        if(d.x == 0) {
            if(p.x < minX || p.x > maxX) return false;
        }
        float txmin = (minX - p.x)/d.x;
        float txmax = (maxX - p.x)/d.x;
        if(txmin > txmax) swap(txmin, txmax);

        tn = std::max(tn, txmin);
        tf = std::min(tf, txmax);
        if(tn > tf || tf < 0) {
            return false;
        }
        t = tn;
        
        /////////////////////////////
        if(d.y == 0) {
            if(p.y < min.y || p.y > max.y) return false;
        }
        float tymin = (min.y - p.y)/d.y;
        float tymax = (max.y - p.y)/d.y;
        if(tymin > tymax) swap(tymin, tymax);

        tn = std::max(tn, tymin);
        tf = std::min(tf, tymax);
        if(tn > tf || tf < 0) {
            return false;
        }
        t = tn;

        /////////////////////////////
        if(d.z == 0) {
            if(p.z < min.z || p.z > max.z) return false;
        }
        float tzmin = (min.z - p.z)/d.z;
        float tzmax = (max.z - p.z)/d.z;
        if(tzmin > tzmax) swap(tzmin, tzmax);

        tn = std::max(tn, tzmin);
        tf = std::min(tf, tzmax);
        if(tn > tf || tf < 0) {
            return false;
        }
        t = tn;

        return true;
    }
};