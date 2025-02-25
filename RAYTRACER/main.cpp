#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <FreeImage.h>
#include <stdio.h>
#include <cassert>
using namespace std;

#define MAINPROGRAM 
#include "scene.h" 
#include "Transform.h"
#include "RayTracer.h"
//#include "shapes.h"

void saveScreenshot(Scene& scene) {
    scene.setCamera();
    RayTracer rayt;
    // RENDER WITH RAYTRACER CLASS
    Image image = rayt.RayTrace(scene, scene.width, scene.height);

    // create an empty FIBITMAP
    FIBITMAP* img = FreeImage_Allocate(scene.width, scene.height, 24);

    // set pixel values from the image to the FIBITMAP
    for (int i = 0; i < scene.height; i++) {
        for (int j = 0; j < scene.width; j++) {
            vec3 pixelColor = image.pixels[i][j];
            RGBQUAD color;
            color.rgbRed = (BYTE)(pixelColor.r * 255);
            color.rgbGreen = (BYTE)(pixelColor.g * 255);
            color.rgbBlue = (BYTE)(pixelColor.b * 255);
            FreeImage_SetPixelColor(img, j, i, &color);
        }
    }
    std::cout << "Saving screenshot: " << scene.fname << "\n";
    FreeImage_Save(FIF_PNG, img, scene.fname.c_str(), 0);
    // delete pixels;
}

void debugInfo(const Scene& scene) {
    cout << "========Scene Information=========\n";
    cout << "Width: " << scene.width << endl;
    cout << "Height: " << scene.height << endl;
    cout << "Max Depth: " << scene.maxdepth << endl;
    cout << "Filename: " << scene.fname << endl;

    cout << "========Camera Information=========\n";
    cout << "Eye: (" << scene.camera.eye.x << ", " << scene.camera.eye.y << ", " << scene.camera.eye.z << ")\n";
    cout << "Center: (" << scene.camera.center.x << ", " << scene.camera.center.y << ", " << scene.camera.center.z << ")\n";
    cout << "Up: (" << scene.camera.up.x << ", " << scene.camera.up.y << ", " << scene.camera.up.z << ")\n";
    cout << "Fovy: " << scene.camera.fovy << endl;

    cout << "Ambient: [" << scene.material.ambient[0] << ", " << scene.material.ambient[1] << ", " << scene.material.ambient[2] << "]\n";
    cout << "Emission: [" << scene.material.emission[0] << ", " << scene.material.emission[1] << ", " << scene.material.emission[2] << "]\n";
    cout << "Diffuse: [" << scene.material.diffuse[0] << ", " << scene.material.diffuse[1] << ", " << scene.material.diffuse[2] << "]\n";
    cout << "Specular: [" << scene.material.specular[0] << ", " << scene.material.specular[1] << ", " << scene.material.specular[2] << "]\n";
    cout << "Shininess: " << scene.material.shininess << endl;

    int sphereCount = 0;
    int triangleCount = 0;
    for (const auto& obj : scene.objects) {
        if (obj->type == 0) { // directional count
            sphereCount++;
        }
        else if (obj->type == 1) { // point count
            triangleCount++;
        }
    }
    cout << "========Object Information=========\n";
    cout << "Sphere: " << sphereCount << endl;
    cout << "Triangle: " << triangleCount << endl;

    cout << "========Vertices Information=========\n";


    int directionalCount = 0;
    int pointCount = 0;
    for (const auto& light : scene.lights) {
        if (light->type == 0) { // directional count
            directionalCount++;
        }
        else if (light->type == 1) { // point count
            pointCount++;
        }
    }
    cout << "========Lighting Information=========\n";
    cout << "Directional: " << directionalCount << endl;
    cout << "Point: " << pointCount << endl;

}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: transforms scenefile [grader input (optional)]\n";
        exit(-1);
    }
    FreeImage_Initialise();

    Scene scene; 
    scene.readfile(argv[1]);
    saveScreenshot(scene);

    debugInfo(scene);

    FreeImage_DeInitialise();
    return 0;
}
