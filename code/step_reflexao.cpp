#include <bits/stdc++.h>
#include "lib/sphere.h"
#include "lib/hitable_list.h"
#include "lib/camera.h"
#include "lib/material.h"
#include "float.h"

using namespace std;

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

int main() {
    int nx = 1000;
    int ny = 500;
    int ns = 1;
    printf("P3\n%i %i\n255\n", nx, ny);
    hitable *list[4];
    float R = cos(M_PI/4.0);
    // materia(cor, ke, kd, ks, alpha)
    list[0] = new sphere(vec3(-R-2, R, -3), R, new material(vec3(0.67, 0.09, 0.09), 0.1, 1.0, 0.6, 0.9)); //vermelha
    list[1] = new sphere(vec3(R,R,-1.0), R, new material(vec3(1, 1, 1), 0.1, 0.5, 10.6, 0.3, vec3(1,1,1))); //branca - vidro
    list[2] = new sphere(vec3(R,-1000,-1.0), 1000, new material(vec3(1, 0.9, 0.8), 0.5, 0.5, 0.6, 1.0)); //chao
    list[3] = new sphere(vec3(R,0.7,-.1), 0.1, new material(vec3(0.14, 0.51, 0.2), 0.1, 0.5, 10.6, 1.1190)); //bolinha
    //list[4] = new sphere(vec3(R+5,40.7,-.1), 3, new material(vec3(0.8, 0.8, 0.3), 0.1, 0.5, 10.6, 1.1190, vec3(1,1,1))); //bola maior espelho
    hitable *world = new hitable_list(list,4);
    vec3 lookfrom(-R-5, R, -2.5);
    vec3 lookat(-R-2, R, -3);
    float dist_to_focus = (lookfrom-lookat).length();
    float aperture = 0.01;
    int fov = 50;
    camera cam(lookfrom, lookat, vec3(0,1,0), fov, float(nx)/float(ny), aperture, dist_to_focus);
    materialLight light(vec3(1.0, 1.0, 1.0), vec3(-10.0, 1000.0, -1.0));
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            vec3 col(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float (j+ drand48()) / float(nx);
                float v = float (i+ drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, light, cam, 0);
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