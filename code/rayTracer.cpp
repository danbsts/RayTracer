#include <bits/stdc++.h>
#include <fstream>
#include "lib/sphere.h"
#include "lib/hitable_list.h"
#include "lib/camera.h"
#include "lib/material.h"
#include "float.h"

using namespace std;

struct materialBuilder{
    vec3 color;
    float ke;
    float kd;
    float ks;
    float alpha;
};

int main() {
    string fileName;
    cerr << "Por favor digite o nome do arquivo a ser aberto:" << endl;
    //cin >> fileName;
    ifstream inFile;
	inFile.open("sample.in");
	if (!inFile) {
    	cerr << "Unable to open file datafile.txt";
    	exit(1);
    }
    string read;
    int nx = 1000;
    int ny = 500;
    int ns = 3;
    inFile >> read >> nx >> ny;
    cerr << nx << " " << ny << endl;
    vec3 lookfrom;
    vec3 lookat;
    vec3 camUp;
    float fov, apperture, focusDist, aspect = float(nx)/float(ny);
    inFile >> read >> lookfrom.e[0] >> lookfrom.e[1] >> lookfrom.e[2];
    cerr << lookfrom.e[0] << " " << lookfrom.e[1] << " " << lookfrom.e[2] << " ";
    inFile >> lookat.e[0] >> lookat.e[1] >> lookat.e[2];
    cerr << lookat.e[0] << " " << lookat.e[1] << " " << lookat.e[2] << " ";
    inFile >> camUp.e[0] >> camUp.e[1] >> camUp.e[2];
    cerr << camUp.e[0] << " " << camUp.e[1] << " " << camUp.e[2] << " ";
    inFile >> fov >> apperture;
    cerr << fov << " " << apperture << endl;
    focusDist = (lookfrom-lookat).length();
    camera view(lookfrom, lookat, camUp, fov, aspect, apperture, focusDist);
    map<string, material> materials;
    inFile >> read;
    while(read == "material") {
        string name;
        inFile >> name;
        material newMat;
        float r, g, b, e, d, s, alp;
        inFile >> r >> g >> b >> e >> d >> s >> alp;
        cerr << read << " " << name << " " << r << " " << g << " " << b << " " << e << " " << d << " " << s << " " << alp << endl;
        newMat.color = vec3(r/255.0, g/255.0, b/255.0);
        newMat.kd = d;
        //cerr << newMat.color << " KD" << endl;
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
        cerr << read << " ";
        string matName;
        float x, y, z, r;
        inFile >> x >> y >> z >> r >> matName;
        sphere newSphere;
        newSphere.center = vec3(x, y, z);
        cerr << newSphere.center << " " << r << " " << matName << endl;
        newSphere.radius = r;
        material& thisMat = materials[matName];
        //cerr << thisMat.color << endl;
        newSphere.mat_ptr = new material(thisMat.color, thisMat.ke, thisMat.kd, thisMat.ks, thisMat.alpha, thisMat.albedo);
        spheres.push_back(newSphere);
        inFile >> read;
    }
    inFile.close();
    int numItens = spheres.size();
    hitable *list[numItens];
    cerr << numItens << endl;
    for(int i = 0; i < spheres.size(); i++) {
        list[i] = new sphere(spheres[i].center, spheres[i].radius, spheres[i].mat_ptr);
        cerr << spheres[i].center << endl;
    }
    //offstream outFile;
    //outFile.open("../out/img.")
    printf("P3\n%i %i\n255\n", nx, ny);
    float R = cos(M_PI/4.0);
    // materia(cor, ke, kd, ks, alpha)
    //list[0] = new sphere(vec3(-R-2, R, -3), R, new material(vec3(0.67, 0.09, 0.09), 0.1, 1.0, 0.6, 0.9)); //vermelha
    //list[1] = new sphere(vec3(R,R,-1.0), R, new material(vec3(1, 1, 1), 0.1, 0.5, 10.6, 0.3, vec3(1,1,1))); //branca - vidro
    //list[2] = new sphere(vec3(R,-1000,-1.0), 1000, new material(vec3(1, 0.9, 0.8), 0.5, 0.5, 0.6, 1.0)); //chao
    //list[3] = new sphere(vec3(R,0.7,-.1), 0.1, new material(vec3(0.14, 0.51, 0.2), 0.1, 0.5, 10.6, 1.1190)); //bolinha
    //list[4] = new sphere(vec3(R+5,40.7,-.1), 3, new material(vec3(0.8, 0.8, 0.3), 0.1, 0.5, 10.6, 1.1190, vec3(1,1,1))); //bola maior espelho
    hitable *world = new hitable_list(list,numItens);
    //vec3 lookfrom(-R-5, R, -2.5);
    //vec3 lookat(-R-2, R, -3);
    //float dist_to_focus = (lookfrom-lookat).length();
    //float aperture = 0.01;
    //int fov = 50;
    //camera view2(lookfrom, lookat, vec3(0,1,0), fov, float(nx)/float(ny), 0.2, focusDist);
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