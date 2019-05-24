#include <bits/stdc++.h>
//#include 'vec3.h'

using namespace std;

int main() {
    int nx, ny;
    scanf("%d %d", &nx, &ny);
    printf("P3\n%i %i\n255\n", nx, ny);
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            float r, g, b;
            r = float(j)/float(nx);
            g = float(i)/float(ny);
            b = 0.2;
            int ir = int(255.99*r), ig = int(255.99*g), ib = int(255.99*b);
            printf("%i %i %i\n", ir, ig, ib);
        }
    }
    return 0;
}