#include <bits/stdc++.h>
#include <fstream>
#include "lib/sphere.h"
#include "lib/hitable_list.h"
#include "lib/camera.h"
#include "lib/material.h"
#include "float.h"

using namespace std;

int main() {
    string fileName;
    ifstream inFile;
	inFile.open("sample.in");
	if (!inFile) {
    	cerr << "Unable to open file datafile.txt";
    	exit(1);
    }
    string read;
    int nx = 1000;
    int ny = 500;
    int ns = 1;
    inFile >> read >> nx >> ny >> ns;
    vec3 lookfrom;
    vec3 lookat;
    vec3 camUp;
    float fov, apperture, focusDist, aspect = float(nx)/float(ny);
    inFile >> read >> lookfrom.e[0] >> lookfrom.e[1] >> lookfrom.e[2];
    inFile >> lookat.e[0] >> lookat.e[1] >> lookat.e[2];
    inFile >> camUp.e[0] >> camUp.e[1] >> camUp.e[2];
    inFile >> fov >> apperture;
    focusDist = (lookfrom-lookat).length();
    camera view(lookfrom, lookat, camUp, fov, aspect, apperture, focusDist);
    map<string, material> materials;
    inFile >> read;
    while(read == "material") {
        string name;
        inFile >> name;
        material newMat;
        float r, g, b, e, d, s, alp;
        inFile >> r >> g >> b >> d >> s >> e >> alp;
        newMat.color = vec3(r/255.0, g/255.0, b/255.0);
        newMat.kd = d;
        newMat.ke = e;
        newMat.ks = s;
        newMat.alpha = alp;
        if(name == "vidro") {
            newMat.albedo = vec3(1,1,1);
        } else {
            newMat.albedo = vec3(0,0,0);
        }
        materials[name] = newMat;
        inFile >> read;
    }
    vector<sphere> spheres;
    while(read == "sphere") {
        string matName;
        float x, y, z, r;
        inFile >> x >> y >> z >> r >> matName;
        sphere newSphere;
        newSphere.center = vec3(x, y, z);
        newSphere.radius = r;
        material& thisMat = materials[matName];
        newSphere.mat_ptr = new material(thisMat.color, thisMat.ke, thisMat.kd, thisMat.ks, thisMat.alpha, thisMat.albedo);
        spheres.push_back(newSphere);
        inFile >> read;
    }
    inFile.close();
    int numItens = spheres.size();
    hitable *list[numItens];
    for(int i = 0; i < spheres.size(); i++) {
        list[i] = new sphere(spheres[i].center, spheres[i].radius, spheres[i].mat_ptr);
    }
    printf("P3\n%i %i\n255\n", nx, ny);
    hitable *world = new hitable_list(list,numItens);
    materialLight light(vec3(1.0, 1.0, 1.0), vec3(-10.0, 100.0, -1.0));
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            vec3 col(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float (j+ drand48()) / float(nx);
                float v = float (i+ drand48()) / float(ny);
                ray r = view.get_ray(u, v);
                col += color(r, world, light, view, 0);
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