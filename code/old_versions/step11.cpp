#include <iostream>
#include <bits/stdc++.h>
#include "lib/sphere.h"
#include "lib/hitable_list.h"
#include "lib/camera.h"
#include "lib/material.h"
#include "float.h"

using namespace std;

vec3 color(const ray& r, const hitable *world, const materialLight& light, const camera& view) {
    hit_record rec;
    if(world->hit(r, 0.000001, FLT_MAX, rec)) {
        hit_record hitted;
        if(world->hit(ray(rec.p, light.position-rec.p), 0.001, FLT_MAX, hitted)) {
            vec3 unit_direction = unit_vector(r.direction());
            float t = 0.5*(unit_direction.y()+1.0);
            return (1.0-t)*vec3(0,0,0) + t*vec3(0.5, 0.7, 1.0);
            return vec3(0.0, 0.0, 0.0);
        }
        return phong(light, rec, view);
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y()+1.0);
    return (1.0-t)*vec3(0,0,0) + t*vec3(0.5, 0.7, 1.0);
    return vec3(0.0, 0.0, 0.0);
}

int main() {
    int nx = 1000;
    int ny = 500;
    int ns = 1;
    printf("P3\n%i %i\n255\n", nx, ny);
    hitable *list[3];
    float R = cos(M_PI/4.0);
    // materia(cor, ke, kd, ks, alpha)
    list[0] = new sphere(vec3(-R, R, -4), R, new material(vec3(0.8, 0.3, 0.3), 0.2, 0.5, 0.6, 0.4)); //vermelha
    list[1] = new sphere(vec3(R,1,-1.0), R, new material(vec3(0.8, 0.8, 0.0), 0.1, 0.5, 0.6, 1.1190)); //verde
    list[2] = new sphere(vec3(R,-100,-1.0), 100, new material(vec3(1, 1, 1), 0.2, 0.5, 0.6, 1.0)); //chao
    hitable *world = new hitable_list(list,3);
    vec3 lookfrom(0,2,-1+5);
    vec3 lookat(R,1,-1.0);
    float dist_to_focus = (lookfrom-lookat).length();
    float aperture = 0.20;
    int fov = 30;
    camera cam(lookfrom, lookat, vec3(0,1,0), fov, float(nx)/float(ny), aperture, dist_to_focus);
    materialLight light(vec3(1.0, 1.0, 1.0), vec3(5.0, 1000.0, 701.0));
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            vec3 col(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float (j+ drand48()) / float(nx);
                float v = float (i+ drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, light, cam);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            printf("%i %i %i\n", ir, ig, ib);
        }
    }
}