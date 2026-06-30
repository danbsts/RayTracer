#ifndef BOXH
#define BOXH

#include "hitable.h"

// Axis-aligned box (cube). Intersection via the slab method; the hit normal is
// the normal of the face the ray enters through.
class box: public hitable {
    public:
        box() {}
        box(vec3 center, float side, material *mat) : mat_ptr(mat) {
            vec3 h(side/2.0, side/2.0, side/2.0);
            bmin = center - h;
            bmax = center + h;
        }
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        vec3 bmin, bmax;
        material *mat_ptr;
};

bool box::hit(const ray &r, float t_min, float t_max, hit_record &rec) const {
    vec3 o = r.origin();
    vec3 d = r.direction();
    float tmin = t_min, tmax = t_max;
    int axis = -1;
    float nsign = 0.0;

    for (int a = 0; a < 3; a++) {
        float invD = 1.0 / d[a];
        float t0 = (bmin[a] - o[a]) * invD;
        float t1 = (bmax[a] - o[a]) * invD;
        float s = -1.0;            // entering the -a face (normal points -a)
        if (invD < 0.0) {          // ray travels in -a, so it enters the +a face
            float tmp = t0; t0 = t1; t1 = tmp;
            s = 1.0;
        }
        if (t0 > tmin) { tmin = t0; axis = a; nsign = s; }
        if (t1 < tmax) { tmax = t1; }
        if (tmax <= tmin) return false;
    }

    if (axis < 0) return false;           // origin inside the box: ignore
    if (tmin < t_min || tmin > t_max) return false;

    rec.t = tmin;
    rec.p = r.point_at_parameter(tmin);
    vec3 n(0, 0, 0);
    n[axis] = nsign;
    rec.normal = n;
    rec.mat_ptr = mat_ptr;
    return true;
}

#endif
