#include <algorithm>
#include <iostream>
#include <stdio.h>

#ifndef SHAPES_H
#define SHAPES_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

struct Ray {
    vec3 origin;
    vec3 direction;
    Ray(const vec3& _origin, const vec3& _direction)
        : origin(_origin), direction(_direction) {}
};


struct Vertex {
    float position[3];
    Vertex(float x, float y, float z) {
        position[0] = x;
        position[1] = y;
        position[2] = z;
    }
};

struct Intersection {
    int hit;
    double t;
    vec3 pos;
    vec3 rdir;
    vec3 rpos;
    vec3 norm;
};

struct Material {
    float ambient[3];
    float specular[3];
    float emission[3];
    float diffuse[3];
    float shininess;
};

class Shapes {
public:
    int type; // 0 for sphere , 1 for triangle
    Material material;
    Shapes() = default;
    Shapes(const Material& material, int _type) : material(material), type(_type) {}
    mat4 transform; // handles transformations
    vec3 normal;
    vec3 sp_positions; 
    float sp_radius; 
    int tri_vertices[3]; 
    vec3 tri_positions[3]; 
};

#endif // SHAPES_H
