#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <stack>
#include <sstream>
#include "Transform.h"
#include <glm/glm.hpp>
#include "shapes.h"

using namespace std;

// void matransform(stack<mat4>& transfstack, GLfloat* values);
void rightmultiply(const mat4& M, stack<mat4>& transfstack); 

struct Image {
    vector<vector<vec3>> pixels;
    Image(int width, int height) : pixels(height, vector<vec3>(width)) {}
    Image() {};
};

// Camera struct
struct Camera {
    vec3 eye;
    vec3 center;
    vec3 up;
    float fovy; // field of view
    float fovx;
    vec3 wvec, uvec, vvec;
    Camera() {};
};

// Light struct
struct Light {
    // Light properties
    float positions[3];
    float colors[3];
    int type; // 0 if directional, 1 if point

    Light(float posX, float posY, float posZ, float colorR, float colorG, float colorB, int lightType) {
        positions[0] = posX;
        positions[1] = posY;
        positions[2] = posZ;

        colors[0] = colorR;
        colors[1] = colorG;
        colors[2] = colorB;

        type = lightType;
    }
};

// Scene class
class Scene {
private:
    bool readvals(stringstream& s, const int numvals, float* values);
public:
    
    Camera camera;
    int width, height, maxdepth;
    string fname = "result.png";
    vector<Light*> lights;
    vector<Shapes*> objects;
    vector<Vertex*> vertices;
    Material material;
    void readfile(const char* filename);
    void setCamera();
    Scene() {
    };

};

#endif // SCENE_H
