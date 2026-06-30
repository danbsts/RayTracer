#ifndef PLANEH
#define PLANEH

#include "hitable.h"

// Infinite plane through point p0 with the given normal. Used as a flat mirror
// when assigned a reflective material. The stored normal is always oriented to
// face the incoming ray so shading and reflection behave on both sides.
class plane: public hitable {
    public:
        plane() {}
        plane(vec3 point, vec3 n, material *mat) : p0(point), mat_ptr(mat) {
            normal = unit_vector(n);
        }
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        vec3 p0;
        vec3 normal;
        material *mat_ptr;
};

bool plane::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    float denom = dot(normal, r.direction());
    if (fabs(denom) < 1e-8) return false;        // ray parallel to plane
    float t = dot(p0 - r.origin(), normal) / denom;
    if (t < t_min || t > t_max) return false;
    rec.t = t;
    rec.p = r.point_at_parameter(t);
    rec.normal = (denom < 0.0) ? normal : -normal;   // face the ray
    rec.mat_ptr = mat_ptr;
    return true;
}

#endif
