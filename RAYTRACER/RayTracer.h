#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <stack>
#include <sstream>
#include "Transform.h"
#include <glm/glm.hpp>
#include "shapes.h"

class RayTracer {
    Image img;
    Ray RayThroughPix(Scene scene, int i, int j);
    vec3 FindColor(Intersection in, Scene scene);
    double findSphereIntersection(Ray r, Shapes sphere);
    double findTriIntersection(Ray r, Shapes triangle);
    Intersection Intersect(Ray r, Scene scene);

    // debugging function
    void PrintRay(const Ray& ray) {
        std::cout << "Ray Origin: (" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << ")\n";
        std::cout << "Ray Direction: (" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << ")\n";
    }

    // debugging function
    void printPixelColors(const Image& image) {
        int width = image.pixels[0].size();
        int height = image.pixels.size();

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                vec3 pixelColor = image.pixels[i][j];
                std::cout << "Pixel at (" << i << ", " << j << "): ";
                std::cout << "R: " << pixelColor.r << ", ";
                std::cout << "G: " << pixelColor.g << ", ";
                std::cout << "B: " << pixelColor.b << std::endl;
            }
        }
    }
    // debugging function to print intersection information
    void PrintIntersection(const Intersection& intersection) {
        std::cout << "Intersection Hit Object: " << intersection.hit << "\n";
        std::cout << "Intersection Position: (" << intersection.pos.x << ", " << intersection.pos.y << ", " << intersection.pos.z << ")\n";
        std::cout << "Intersection Normal: (" << intersection.norm.x << ", " << intersection.norm.y << ", " << intersection.norm.z << ")\n";
        std::cout << "Intersection T: " << intersection.t << "\n";
    }

    // Debugging function to print computed light information
    void PrintLighting(const vec4& lightResult) {
        std::cout << "Computed Light Result: (" << lightResult.x << ", " << lightResult.y << ", " << lightResult.z << ", " << lightResult.w << ")\n";
    }
public:
    Image RayTrace(Scene scene, int w, int h);
    vec4 ComputeLight(const vec3 direction, const vec4 lightcolor, const vec3 normal, const vec3 halfvec, const vec4 mydiffuse, const vec4 myspecular, const float myshininess);
    RayTracer() {

    };
};

#endif 