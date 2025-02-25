#include "shapes.h"

// placeholder function to compute color based on material properties and intersection point
vec3 computeColor(const Material& material, const vec3& intersectionPoint) {
    vec3 ambientColor(material.ambient[0], material.ambient[1], material.ambient[2]);

    // diffuse term
    vec3 diffuseColor(material.diffuse[0], material.diffuse[1], material.diffuse[2]);

    // specular term
    vec3 specularColor(material.specular[0], material.specular[1], material.specular[2]);

    // emission term
    vec3 emissionColor(material.emission[0], material.emission[1], material.emission[2]);

    // shininess factor
    float shininess = material.shininess;

    float nDotL = dot(normal, direction);
    vec3 lambert = mydiffuse * lightcolor * max(nDotL, 0.0);

    float nDotH = dot(normal, halfvec);
    vec3 phong = specularColor * lightcolor * pow(max(nDotH, 0.0), shininess);

    vec3 color = lambert + phong;

    // compute final color using Phong shading model
    //vec3 color = ambientColor + diffuseColor + specularColor + emissionColor;

    return color;
}
