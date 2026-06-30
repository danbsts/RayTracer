#ifndef MATERIALH
#define MATERIALH

struct hit_record;

#include "ray.h"
#include "hitable.h"
#include "rng.h"

using namespace std;

// A point inside the unit sphere — used to jitter the area light for soft shadows.
inline vec3 random_in_unit_sphere(RNG& rng) {
    vec3 p;
    do {
        p = 2.0*vec3(rng(), rng(), rng()) - vec3(1, 1, 1);
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
            vec3 d = unit_vector(r_in.direction());
            vec3 n = unit_vector(rec.normal);
            vec3 reflected = d - 2.0f*dot(d, n)*n;   // specular reflection about the normal
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return dot(reflected, n) > 0;
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
        materialLight(vec3 cor, vec3 dir) : color(cor), position(dir), radius(0.0) {}
        materialLight(vec3 cor, vec3 dir, float rad) : color(cor), position(dir), radius(rad) {}

        vec3 color;
        vec3 position;
        float radius;   // area-light size; 0 = hard point light
};

inline float max(float a, float b) {
    if(a > b) return a;
    return b;
}

vec3 reflect(const vec3 &v, const vec3 &n) {
    return 2*dot(v,n)*n - v;
}

// Ambient coefficient — the fill light a surface receives even in full shadow,
// so shadows fall to a soft tone instead of pure black.
const float AMBIENT = 0.18f;

// Phong shading with an ambient term. When 'lit' is false the point is in shadow
// and only receives ambient (+ emissive); diffuse/specular are dropped.
vec3 shade(const materialLight& light, const hit_record& rec, const camera& view, bool lit) {
    vec3 baseColor = rec.mat_ptr->color;
    vec3 emissive = rec.mat_ptr->ke * baseColor * light.color;
    vec3 ambient  = AMBIENT * baseColor;
    if (!lit) return emissive + ambient;

    vec3 L = unit_vector(light.position - rec.p);
    vec3 V = unit_vector(view.origin - rec.p);
    vec3 N = unit_vector(rec.normal);
    float cosTheta = max(0.0f, dot(N, L));

    vec3 diffuse(0, 0, 0), specular(0, 0, 0);
    if (cosTheta > 0.0) {
        vec3 R = reflect(L, N);
        diffuse  = rec.mat_ptr->kd * baseColor * light.color * cosTheta;
        specular = rec.mat_ptr->ks * baseColor * light.color *
                   pow(max(0.0, dot(V, R)), rec.mat_ptr->alpha*128.0);
    }
    return emissive + ambient + diffuse + specular;
}

vec3 color(const ray& r, const hitable *world, const materialLight& light, const camera& view, int depth, RNG& rng) {
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        if(depth < 10 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, light, view, depth+1, rng);
        }
        // Soft shadow: cast one shadow ray toward a random point on the area
        // light (radius). The ray stops at the light (t in (0.001, 1)), so
        // objects behind the light don't occlude. Averaging this binary test
        // over the pixel's many AA samples yields a smooth penumbra gradient.
        hit_record hitted;
        vec3 lightPoint = light.position + light.radius * random_in_unit_sphere(rng);
        bool lit = !world->hit(ray(rec.p, lightPoint - rec.p), 0.001, 1.0, hitted);
        return shade(light, rec, view, lit);
    }

    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y()+1.0);
    return (1.0-t)*vec3(0,0,0) + t*vec3(0.5, 0.7, 1.0);
}

#endif
