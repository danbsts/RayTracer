#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hitable.h"

using namespace std;

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

class material {
    public:
        material() {}
        material(vec3 c, float e, float d, float s, float al) : color(c), ke(e), ks(s), kd(d), alpha(al), albedo(vec3(0,0,0)) {}
        material(vec3 c, float e, float d, float s, float al, vec3 alb) : color(c), ke(e), ks(s), kd(d), alpha(al), albedo(alb) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
            if(albedo.x() == 0 && albedo.y() == 0 && albedo.z() == 0) {
                return false;
            }
            //vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            vec3 target = rec.p + rec.normal;
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }

        vec3 albedo;
        vec3 color;
        float ke;
        float ks;
        float kd;
        float alpha;
};

class materialLight {
    public:
        materialLight() {}
        materialLight(vec3 cor, vec3 dir) : color(cor), position(dir) {}

        vec3 color;
        vec3 position;
};

inline float max(float a, float b) {
    if(a > b) return a;
    return b;
}

vec3 reflect(const vec3 &v, const vec3 &n) {
    return 2*dot(v,n)*n - v;
}

vec3 phong(materialLight light, const hit_record& rec, const camera& view) {
    vec3 lightDirection = unit_vector(light.position - rec.p);
    vec3 viewDirection = unit_vector(view.origin - rec.p);
    vec3 normaNormal = unit_vector(rec.normal);

    float cosTheta = max(0.0f, dot(normaNormal, lightDirection));

    //cores
    vec3 emissive = rec.mat_ptr->ke*rec.mat_ptr->color*light.color;
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 relfectionDir = reflect(lightDirection, normaNormal);

    if(cosTheta > 0.0) {
        diffuse = rec.mat_ptr->kd * rec.mat_ptr->color * light.color * cosTheta;
        specular = rec.mat_ptr->ks * rec.mat_ptr->color * light.color * pow(max(0.0, dot(viewDirection, relfectionDir)), rec.mat_ptr->alpha*128.0);
    }

    //return specular;
    //return diffuse;
    //return emissive;
    return emissive + diffuse + specular;
}

vec3 color(const ray& r, const hitable *world, const materialLight& light, const camera& view, int depth) {
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        hit_record hitted;
        ray scattered;
        vec3 attenuation;
        if(depth < 10) {
            if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
                return attenuation*color(scattered, world, light, view, depth+1);
            } else { 
                if(world->hit(ray(rec.p, light.position-rec.p), 0.001, FLT_MAX, hitted)) {
                    return vec3(0,0,0);
                } else {
                    return phong(light, rec, view);
                }
            }
        } else {
            if(world->hit(ray(rec.p, light.position-rec.p), 0.001, FLT_MAX, hitted)) {
                 //return phong(light, rec, view);
                 return vec3(0.0, 0.0, 0.0);
            } else {
                return phong(light, rec, view);
            }
        }
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y()+1.0);
    return (1.0-t)*vec3(0,0,0) + t*vec3(0.5, 0.7, 1.0);
}

#endif