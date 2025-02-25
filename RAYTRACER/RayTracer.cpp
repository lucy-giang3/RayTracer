#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <vector>
#include <stack>
#include "Transform.h"
#include "shapes.h"
#include <glm/glm.hpp>
using namespace std;
using namespace glm;
#include "scene.h"
#include "RayTracer.h"

Image RayTracer::RayTrace(Scene scene, int w, int h) {
    Image image(w, h);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            Ray r = RayThroughPix(scene, i, j);
            // PrintRay(r);
            Intersection hit = Intersect(r, scene);
            // PrintIntersection(hit);
            image.pixels[i][j] = FindColor(hit, scene);
        }
    }
    // printPixelColors(image);
    return image;
}

Ray RayTracer::RayThroughPix(Scene scene, int i, int j) {
    // convert width and height int to double
    float w = (float)(scene.width);
    float h = (float)(scene.height);
    float fovycam = (scene.camera.fovy);
    // formula from discussion
    // a = (eye - center)
    // b = up
    float alpha = tan(fovycam / 2.0) * w / h * (j + 0.5 - w / 2) / (w / 2);
    float beta = tan(fovycam / 2.0) * (h / 2 - i - 0.5) / (h / 2);
    vec3 dir = (alpha * scene.camera.uvec) + (beta * scene.camera.vvec) - (scene.camera.wvec);
    vec3 origin = scene.camera.eye; // p0 = camera or eye position
    // make sure to normalize
    return Ray(origin, normalize(dir));
}

double RayTracer::findTriIntersection(Ray r, Shapes triangle) {

    // set up variables
    vec3 A = triangle.tri_positions[0];
    vec3 B = triangle.tri_positions[1];
    vec3 C = triangle.tri_positions[2];

    // n = (C-A) x (B-A) / |(C-A) x (B-A)|
    vec3 n = glm::normalize(glm::cross(C - A, B - A));

    // P0 : origin , P1 : direction
    // Intersection = t = (A * n - P0 * n) / (P1 * n)
    float numerator = glm::dot(A, n) - glm::dot(r.origin, n);
    float denominator = glm::dot(r.direction, n);
    float t = numerator / denominator;
    // if there is ray-plane intersection
    if (t < 0) {
        return -1;
    }
    //cout << "t: " << t << endl;

    // ray : P = P0 + tP1
    vec3 P = r.origin + (t * r.direction);

    // determine whether intersection is in plane
    // P-A = b(B - A) + v(C-A)
    vec3 P_minus_A = P - A;
    vec3 B_minus_A = B - A;
    vec3 P_minus_B = P - B;
    vec3 C_minus_B = C - B;
    vec3 P_minus_C = P - C;
    vec3 A_minus_C = A - C;

    vec3 cross1 = glm::cross(P_minus_A, B_minus_A);
    vec3 cross2 = glm::cross(P_minus_B, C_minus_B);
    vec3 cross3 = glm::cross(P_minus_C, A_minus_C);

    if (glm::dot(n, cross1) >= 0 && glm::dot(n, cross2) >= 0 && glm::dot(n, cross3) >= 0) {
        // Intersection point P is within the triangle
        return t;
    }
    else {
        // Intersection point P is not within the triangle
        return -1;
    }
}

// p(t) = e + td
double RayTracer::findSphereIntersection(Ray r, Shapes sphere) {
    // sphere properties
    double radius = sphere.sp_radius;
    vec3 center = sphere.sp_positions;

    // ray properties
    vec3 e = r.origin;
    vec3 d = r.direction;

    vec3 e_minus_c = e - center;
    double a = glm::dot(d, d);
    double b = 2.0 * glm::dot(d, e_minus_c);
    double c = glm::dot(e_minus_c, e_minus_c) - radius * radius;

    // compute the discriminant
    double det = b * b - 4 * a * c;

    // check for no intersection
    if (det < 0) {
        return -1.0;
    }

    double t1 = (-b + sqrt(det)) / (2.0 * a);
    double t2 = (-b - sqrt(det)) / (2.0 * a);

    double t;
    if (t1 >= 0 && t2 >= 0) {
        t = std::min(t1, t2);
    }
    else if (t1 >= 0) {
        t = t1;
    }
    else {
        t = t2;
    }

    return t;
}

Intersection RayTracer::Intersect(Ray r, Scene scene) {
    Intersection info;
    double minDist = numeric_limits<double>::max();
    int hitObject = -1;
    //cout << "size: " << scene.objects.size() << endl;
    for (size_t i = 0; i < scene.objects.size(); ++i) {
        Shapes* obj = scene.objects[i];
        // cout << "i: " << i << endl;
        if (obj->type == 0) { // Sphere
            double t = findSphereIntersection(r, *obj);
            //cout << "-------CASE X--------\n";
            if (t > 0 && t < minDist) {
                minDist = t;
                hitObject = i;
                //cout << "-------CASE X.1--------\n";
            }
        }
        else if (obj->type == 1) { // Triangle
            // cout << "-------CASE Y--------\n";
            double t = findTriIntersection(r, *obj);
            //cout << "t: " << t << endl;
            if (t > 0) { //  && t < minDist
                // cout << "-------CASE Y.1--------\n";
                minDist = t;
                hitObject = i;
            }
        }
    }

    if (hitObject != -1) {
        info.hit = hitObject;
        info.t = minDist;
        //cout << "-------INTERSECTION DETECTED--------\n";
        // set intersection point and normal based on the hit object type
        Shapes* hitObj = scene.objects[hitObject];
        if (hitObj->type == 0) { // Sphere
            info.pos = r.origin + (float)(minDist)*r.direction;
            info.norm = normalize(info.pos - hitObj->sp_positions);
        }
        else if (hitObj->type == 1) { // Triangle
            info.pos = r.origin + (float)(minDist)*r.direction;
            info.norm = hitObj->normal;
        }
    }
    else {
        // no intersection detected
        info.hit = -1;
        info.t = -1;
        //cout << "NO INTERSECTION DETECTED\n";
    }
    return info;
}

vec4 RayTracer::ComputeLight(const vec3 direction, const vec4 lightcolor, const vec3 normal, const vec3 halfvec, const vec4 mydiffuse, const vec4 myspecular, const float myshininess) {
    float nDotL = glm::dot(normal, direction);
    vec4 lambert = mydiffuse * lightcolor * std::max(nDotL, 0.0f);

    float nDotH = glm::dot(normal, halfvec);
    vec4 phong = myspecular * lightcolor * pow(std::max(nDotH, 0.0f), myshininess);

    vec4 retval = lambert + phong;
    return retval;
}


vec3 RayTracer::FindColor(Intersection hit, Scene scene) {
    if (hit.hit == -1) {
        // if no intersection, return background color
        return vec3(0.0f, 0.0f, 0.0f);
    }

    vec3 color(0.0f);
    Shapes* obj = scene.objects[hit.hit];

    // Ambient color
    for (int i = 0; i < 3; ++i) {
        color[i] += obj->material.ambient[i];
        //cout << "Ambient: " << obj->material.ambient[i] << endl;
    }

    // Diffuse and specular lighting
    for (size_t i = 0; i < scene.lights.size(); ++i) {
        Light* light = scene.lights[i];

        vec3 L, H;
        if (light->type == 0) {
            // for directional light
            L = -normalize(vec3(light->positions[0], light->positions[1], light->positions[2]));
        }
        else {
            // for point light
            L = normalize(vec3(light->positions[0], light->positions[1], light->positions[2]) - hit.pos);
        }

        H = normalize(L + normalize(scene.camera.eye - hit.pos));

        // "pass to shader" compute light color
        vec4 lightColor(light->colors[0], light->colors[1], light->colors[2], 1.0f);
        vec4 diffuse = vec4(obj->material.diffuse[0], obj->material.diffuse[1], obj->material.diffuse[2], 1.0f);
        vec4 specular = vec4(obj->material.specular[0], obj->material.specular[1], obj->material.specular[2], 1.0f);
        float shininess = obj->material.shininess;

        vec4 lightResult = ComputeLight(L, lightColor, hit.norm, H, diffuse, specular, shininess);

        for (int j = 0; j < 3; ++j) {
            color[j] += lightResult[j];
        }
    }

    return color;
}
