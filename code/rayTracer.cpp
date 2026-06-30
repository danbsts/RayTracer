#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "lib/rng.h"
#include "lib/sphere.h"
#include "lib/plane.h"
#include "lib/hitable_list.h"
#include "lib/camera.h"
#include "lib/material.h"

using namespace std;

// Random generator used for sampling, shared across the render.
RNG g_rng(20240624u);

// Usage: rayTracer [scene_file] [output_ppm]
//   scene_file  defaults to "sample.in"
//   output_ppm  if omitted, the image is written to stdout
int main(int argc, char** argv) {
    const char* sceneFile = (argc > 1) ? argv[1] : "sample.in";
    ifstream inFile;
    inFile.open(sceneFile);
    if (!inFile) {
        cerr << "Unable to open scene file: " << sceneFile << endl;
        exit(1);
    }
    FILE* out = stdout;
    if (argc > 2) {
        out = fopen(argv[2], "w");
        if (!out) { cerr << "Unable to open output file: " << argv[2] << endl; exit(1); }
    }

    string read;
    int nx = 1000, ny = 500, ns = 1;
    inFile >> read >> nx >> ny >> ns;
    vec3 lookfrom, lookat, camUp;
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
        newMat.kd = d; newMat.ke = e; newMat.ks = s; newMat.alpha = alp;
        newMat.albedo = (name == "vidro") ? vec3(1,1,1) : vec3(0,0,0);
        materials[name] = newMat;
        inFile >> read;
    }

    auto matCopy = [&](const string& nm) {
        material& m = materials[nm];
        return new material(m.color, m.ke, m.kd, m.ks, m.alpha, m.albedo);
    };

    // Scene objects. Each line is: keyword, numeric params, then (for non-mirror
    // shapes) a material name. Supported keywords and their parameters:
    //   sphere cx cy cz radius        materialName   -- a sphere
    //   plane  px py pz nx ny nz       materialName   -- matte plane (point + normal)
    //   cube   cx cy cz side          materialName   -- axis-aligned cube (center + edge length)   [NOT IMPLEMENTED]
    //   mirror px py pz nx ny nz                      -- reflective plane (point + normal)          [NOT IMPLEMENTED]
    vector<hitable*> objs;
    while(read == "sphere" || read == "cube" || read == "mirror" || read == "plane") {
        if (read == "sphere") {
            string matName; float x, y, z, r;
            inFile >> x >> y >> z >> r >> matName;
            objs.push_back(new sphere(vec3(x, y, z), r, matCopy(matName)));
        } else if (read == "plane") { // matte colored plane (floor): point + normal + material
            string matName; float px, py, pz, nx, ny, nz;
            inFile >> px >> py >> pz >> nx >> ny >> nz >> matName;
            objs.push_back(new plane(vec3(px, py, pz), vec3(nx, ny, nz), matCopy(matName)));
        } else if (read == "cube") { // not implemented yet: consume params and skip
            string matName; float x, y, z, s;
            inFile >> x >> y >> z >> s >> matName;
            cerr << "cube: not implemented" << endl;
        } else { // mirror: not implemented yet: consume params and skip
            float px, py, pz, nx, ny, nz;
            inFile >> px >> py >> pz >> nx >> ny >> nz;
            cerr << "mirror: not implemented" << endl;
        }
        inFile >> read;
    }
    inFile.close();

    int numItens = objs.size();
    hitable *world = new hitable_list(objs.data(), numItens);
    // Area light (position + radius). The radius gives soft, gradient shadows.
    materialLight light(vec3(1.0, 1.0, 1.0), vec3(-6.0, 12.0, 5.0), 2.5);

    fprintf(out, "P3\n%i %i\n255\n", nx, ny);
    for(int i = ny-1; i > -1; i--) {
        for(int j = 0; j < nx; j++) {
            vec3 col(0,0,0);
            for(int s = 0; s < ns; s++) {
                float u = float(j + g_rng()) / float(nx);
                float v = float(i + g_rng()) / float(ny);
                ray r = view.get_ray(u, v, g_rng);
                col += color(r, world, light, view, 0, g_rng);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            fprintf(out, "%i %i %i\n", ir, ig, ib);
        }
    }
    if (out != stdout) fclose(out);
    return 0;
}
