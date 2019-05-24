#include <iostream>
#include <bits/stdc++.h>
#include "lib/ray.h"

vec3 color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
}

int main() {
    int nx = 200, ny = 100;
    printf("P3\n%i %i\n255\n", nx, ny);
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4, 0, 0);
    vec3 vertical(0, 2, 0);
    vec3 origin(0, 0, 0);
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            float u, v;
            u = float(j)/float(nx);
            v = float(i)/float(ny);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);
            int ir = int(255.99*col[0]), ig = int(255.99*col[1]), ib = int(255.99*col[2]);
            printf("%i %i %i\n", ir, ig, ib);
        }
    }
}