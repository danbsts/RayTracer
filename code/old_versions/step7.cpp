#include <iostream>
#include <bits/stdc++.h>
#include "lib/sphere.h"
#include "lib/hitable_list.h"
#include "lib/camera.h"
#include "float.h"

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world) {
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        vec3 target = rec.p + rec.normal+ random_in_unit_sphere();
        return 0.5*color(ray(rec.p, target-rec.p), world);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y()+1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    printf("P3\n%i %i\n255\n", nx, ny);
    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0.0,-100.5,-1.0), 100);
    hitable *world = new hitable_list(list,2);
    camera cam;
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            vec3 col(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float (j+ drand48()) / float(nx);
                float v = float (i+ drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }
            col /= float(ns);
            //step7_2
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            printf("%i %i %i\n", ir, ig, ib);
        }
    }
}