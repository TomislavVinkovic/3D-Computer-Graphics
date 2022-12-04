#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include "geometry.h"
#include "ray.h"
#include "objects.h"
#include "light.h"

using namespace std;

typedef vector<Vec3f> Image;
typedef vector<Object*> Objects;
typedef vector<Light*> Lights;

// funkcija koja ispisuje sliku u .ppm file
void save_image(const Image &image, const int width, const int height, const string path)
{
    ofstream ofs;
    ofs.open(path, ofstream::binary);
    
    // format ppm
    ofs << "P6\n" << width << " " << height << "\n255\n";
    
    // ispis pixela
    for (int i = 0; i < width * height; ++i)
    {
        ofs << (char)(255 * min(max(image[i][0], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][1], 0.f), 1.f));
        ofs << (char)(255 * min(max(image[i][2], 0.f), 1.f));
    }
    
    // zatvori file
    ofs.close(); 
}

// funkcija koja provjerava sijece li zraka jedan od objekata
bool scene_intersect(const Ray &ray, const Objects &objs, Material &hit_material, Vec3f &hit_point, Vec3f &hit_normal)
{
    float best_dist = numeric_limits<float>::max();
    float dist = numeric_limits<float>::max();
    
    Vec3f normal{};
    for (auto obj : objs)
    {
        if (obj->ray_intersect(ray, dist, normal) && dist < best_dist)
        {
            best_dist = dist;             // udaljenost do sfere
            hit_material = obj->material; // materijal pogodjenog objekta
            hit_normal = normal;          // normala na pogodjeni objekt
            hit_point = ray.origin + ray.direction * dist; // pogodjena tocka
            
        }
    }
    //cout << best_dist << endl;
    return best_dist < 1000;
}

// funkcija koja vraca boju
Vec3f cast_ray(const Ray &ray, const Objects &objs, const Lights &lights, unsigned rec_depth = 0)
{
    Vec3f hit_normal;
    Vec3f hit_point;
    Material hit_material;
    //ako sa zrakom bas potpuno promasimo
    if (!scene_intersect(ray, objs, hit_material, hit_point, hit_normal) || rec_depth > 5)
    {
        return Vec3f(0.8, 0.8, 1); // vrati boju pozadine
    }
    else
    {
        float diffuse_light_intensity = 0;
        float specular_light_intensity = 0;
        Vec3f refl_vector{0,0,0};
        Vec3f refr_vector{0,0,0};
        Ray refr_ray;
        Ray refl_ray;
        bool refr_init = false, refl_init = false;
        for (auto light : lights) 
        {           
            Vec3f light_dir = (light->position - hit_point).normalize();
            float light_dist = (light->position - hit_point).norm();
            
            // SJENE
            // ideja: - rekurzivno pozovi scene_intersect od objekta do svijetla
            //        - ako se nesto nalazi izmedju svjetla i objekta, 
            //          tada to svijetlo ignoriramo
            Material shadow_hit_material;
            Vec3f shadow_hit_normal;
            Vec3f shadow_hit_point;
            
            // zbog gresaka u zaokrizivanju moze se dogoditi da zraka zapocne
            // unutar samog objekta. Da to izbjegnemu, origin zrake  za malo
            // pomicemo u smjeru zrake
            Vec3f shadow_origin;
            if (light_dir * hit_normal < 0) // skalarni produkt je manji od 0 ako su suprotne orijentacije
            {                               
                shadow_origin = hit_point - hit_normal * 0.001;
            }
            else
            {
                shadow_origin = hit_point + hit_normal * 0.001;
            }
            Ray shadow_ray(shadow_origin, light_dir);
            // provjeri hoce li zraka shadow_ray presijecati objekt
            if (scene_intersect(shadow_ray, objs, shadow_hit_material, shadow_hit_point, shadow_hit_normal))
            {
                // zraka sijece neki objekt 
                // trebamo jos provjeriti zaklanja li taj objekt svjetlo
                // tj. nalazi li se izmedju hit_point i light->position
                float dist = (shadow_hit_point - hit_point).norm();
                if (dist < light_dist)
                {
                    // objekt zaklanja svijetlo, preskacemo ovu iteraciju
                    continue;
                }
            }
            //if zraka sjene p + sl nema sjeciste s nijednim objektom na sceni
            // I / r^2
            float dist_factor = light->intensity / (light_dist * light_dist);
            // difuzno sjenacanje (Lambertov model)
            diffuse_light_intensity +=  hit_material.diffuse_coef * dist_factor * max(0.f, hit_normal * light_dir);

            //spekularno sjencanje(Blinn-Phongov model)

            //izracunaj poluvektor za blinn-phonga
            Vec3f view_direction = (ray.origin - hit_point).normalize();
            Vec3f h = (view_direction + light_dir).normalize();
            specular_light_intensity += hit_material.spec_coef * dist_factor * powf(max(0.0f, hit_normal * h), hit_material.phong_exp);

            ////refleksije
            if(hit_material.mirroring_intensity > 0.0f){
                refl_init = true;
                Vec3f r = hit_normal * (ray.direction * hit_normal) * (-2);
                r = r + ray.direction;
                refl_ray = Ray(hit_point + hit_normal * 0.01, r);
            }

            //refrakcije
            {
                refr_init = true;
                float cosi = hit_normal * ray.direction;
                float eta = 2.0f - hit_material.refr_coef;
                Vec3f r = (ray.direction * eta - hit_normal * (-cosi + eta * cosi));
                refr_ray = Ray(hit_point + hit_normal * 0.001, r);
            }
            
        }
        Vec3f diffuse_color = diffuse_light_intensity * hit_material.diffuse_color;
        Vec3f spec_color = specular_light_intensity * Vec3f{1,1,1};
        Vec3f hit_color = diffuse_color + spec_color;
        if(refl_init == true) {
            refl_vector = cast_ray(refl_ray, objs, lights, rec_depth + 1);
        }
        if(refr_init == true) {
            refr_vector = cast_ray(refl_ray, objs, lights, rec_depth + 1);
        }
        return (hit_material.opacity * ((1-hit_material.mirroring_intensity) * hit_color + hit_material.mirroring_intensity * refl_vector)) + 
        ((1-hit_material.opacity) * refr_vector);
    }
}

// funkcija koja napravi zraku iz točke origin
// koja prolazi kroz pixel (i, j) na slici
// (formula s predavanja 3)
Ray ray_to_pixel(Vec3f origin, int i, int j, int width, int height)
{
    Ray ray = Ray();
    ray.origin = origin;
    
    float fov = 1.855; // 106.26° u radijanima
    float tg = tan(fov / 2.);
    
    float x =  (-1 + 2 * (i + 0.5) / (float)width) * tg;
    float y = -(-1 + 2 * (j + 0.5) / (float)height);
    float z = -1;
    
    ray.direction = Vec3f(x, y, z).normalize();
    return ray;
}

void draw_image(Objects objs, Lights lights)
{
    // dimenzije slike
    const int width = 1024;
    const int height = 768;
    
    Image img(width * height);
    
    // ishodište zrake
    Vec3f origin = Vec3f(0, 0, 0);
    
    // crtanje slike, pixel po pixel
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            Ray ray = ray_to_pixel(origin, i, j, width, height);
            Vec3f color = cast_ray(ray, objs, lights);
            img[i + j * width] = color;
        }
    }
    
    // snimi sliku na disk
    save_image(img, width, height, "./render_relective.ppm");
}

int main()
{
    // definiraj materijale
    Material red(Vec3f(1.0F, 0, 0));
    red.spec_coef = 1.0F;
    red.phong_exp = 10;
    red.mirroring_intensity = 0.3f;
    red.opacity = 0.5f;
    
    Material green(Vec3f(0, 0.5, 0));
    green.spec_coef = 1;
    green.phong_exp = 100;
    green.mirroring_intensity = 0.6f;
    green.opacity = 1.0f;

    Material blue(Vec3f(0, 0, 1));
    blue.spec_coef = 0;
    blue.phong_exp = 1;
    blue.mirroring_intensity = 0.5;

    Material grey(Vec3f(0.4f, 0.4f, 0.4f));
    grey.phong_exp = 10;
    grey.mirroring_intensity = 0.4;
    grey.spec_coef = 0.7;

    Material purple({0.5, 0, 1});
    purple.phong_exp = 70.0f;
    purple.spec_coef = 1;
    purple.refr_coef = 1.037;
    //purple.opacity = 0.3f;
    
    // definiraj svjetla
    Light l1 = Light(Vec3f(-20, 20, 20), 1500);
    Light l2 = Light(Vec3f(20, 30, 20), 1800);
    Lights lights = { &l1, &l2 };

    Sphere sphere1({-1.5, -3.0, -12}, 1.5, green);
    Sphere sphere2({3, -4.0, -9}, 0.5, blue);
    Sphere sphere3({-4.0, -2.5, -18.5}, 2.0, purple);
    Cuboid cuboid({3, -4.5, -22}, {8, 3.5, -12}, red);
    Cuboid surface(Vec3f(-30, -5.0, -30), Vec3f(30, -4.5, 9), grey);

    //Objects objs {&sphere1, &sphere2, &sphere3,  &cuboid};
    //Objects objs {&cuboid, &sphere1, &sphere2, &sphere3, &surface};
    Objects objs{&cuboid, &sphere1, &sphere2, &sphere3, &surface};

    draw_image(objs, lights);
    return 0;
    return 0;
}
