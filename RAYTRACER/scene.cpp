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
void rightmultiply(const mat4 & M, stack<mat4> &transfstack)
{
    mat4 &T = transfstack.top();
    T = T * M;
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.
bool Scene::readvals(stringstream &s, const int numvals, float* values)
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void Scene::readfile(const char* filename)
{
    string str, cmd, fname;
    ifstream in;
    in.open(filename);
    if (in.is_open()) {

        stack <mat4> transfstack;
        transfstack.push(mat4(1.0));  // identity

        getline (in, str);
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                stringstream s(str);
                s >> cmd;
                int i;
                float values[10]; // Position and color for light, colors for others ADJUST GLFloat? (can value[] hold strings)
                                    // Up to 10 params for cameras.
                bool validinput; // Validity of input

                if (cmd == "output") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        fname = values[0];
                    }
                }
                else if (cmd == "directional") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        Light* lightptr = new Light(values[0], values[1], values[2], values[3], values[4], values[5], 0);
                        lights.push_back(lightptr);
                    }
                } else if (cmd == "point") {
                    validinput = readvals(s, 6, values);
                    if (validinput) {
                        Light* lightptr = new Light(values[0], values[1], values[2], values[3], values[4], values[5], 1);
                        lights.push_back(lightptr);
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        // do nothing for now
                    }
                }

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            material.ambient[i] = values[i];
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            material.diffuse[i] = values[i];
                        }
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            material.specular[i] = values[i];
                        }
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            material.emission[i] = values[i];
                        }
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        material.shininess = values[0];
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2,values);
                    if (validinput) {
                        width = (int) values[0]; height = (int) values[1];
                    }
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
                        camera.eye = vec3(values[0], values[1], values[2]);
                        camera.center = vec3(values[3], values[4], values[5]);
                        camera.up = vec3(values[6], values[7], values[8]);
                        camera.fovy = values[9];
                    }
                } else if (cmd == "maxdepth") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        maxdepth = values[0];
                    }
                }
                else if (cmd == "maxverts" || cmd == "maxvertnorms") {
                    // do nothing
                }
                else if (cmd == "vertex") {
                    validinput = readvals(s, 3, values);
                    if (validinput) {
                        // create a vertex using the parsed coordinates and add it to a container
                        Vertex* vertex = new Vertex(values[0], values[1], values[2]);
                        vertices.push_back(vertex);
                    }
                }
                else if (cmd == "vertexnormal") {
                    // do nothing for now

                }
                else if (cmd == "sphere" || cmd == "tri") {
                    //cout << "=======ADDED OBJ=========\n";
                    Shapes* obj = new Shapes();
                    obj->material = material;
                    if (cmd == "sphere") {
                        validinput = readvals(s, 4, values);
                        if (validinput) {
                            obj->type = 0;
                            obj->sp_positions = vec3(values[0], values[1], values[2]);
                            obj->sp_radius = values[3];
                        }
                    }
                    else {
                        validinput = readvals(s, 3, values);
                        if (validinput) {
                            obj->type = 1;
                            for (int k = 0; k < 3; k++) {
                                obj->tri_vertices[k] = static_cast<int>(values[k]);
                                obj->tri_positions[k] = vec3(vertices[obj->tri_vertices[k]]->position[0],
                                    vertices[obj->tri_vertices[k]]->position[1],
                                    vertices[obj->tri_vertices[k]]->position[2]);
                            }

                            obj->normal = normalize(cross(obj->tri_positions[1] - obj->tri_positions[0], obj->tri_positions[2] - obj->tri_positions[0]));
                        }
                    }
                    // Set the object's transform
                    // cout << "=======ADDED OBJ=========\n";
                    obj->transform = transfstack.top();
                    // add obj to vector<Shapes*> objects
                    objects.push_back(obj);
                }
                else if (cmd == "trinormal") {
                    // do nothing for now
                    // each vertex has an associated normal, and when doing shading, you should interpolate the normals for intermediate points on the triangle.
                }
                else if (cmd == "translate") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values);
                    if (validinput) {
                        rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values);
                    if (validinput) {
                        vec3 axis(values[0], values[1], values[2]);
                        // normalize before applying rotation
                        mat3 rotationMatrix = Transform::rotate(values[3], glm::normalize(axis));

                        // convert 3x3 matrix to 4x4 matrix
                        mat4 rotationMatrix4 = mat4(rotationMatrix);

                        // apply rotation to top of stack
                        rightmultiply(rotationMatrix4, transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top());
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n";
                    } else {
                        transfstack.pop();
                    }
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline (in, str);
        }
    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        throw 2;
    }
}

void Scene::setCamera() {
    camera.fovy = glm::radians(camera.fovy);
    double aspectRatio = (double)width / (double)height;
    camera.fovx = aspectRatio * camera.fovy;
    camera.wvec = glm::normalize(camera.eye - camera.center);
    camera.uvec = glm::cross(camera.up, camera.wvec);
    camera.uvec = glm::normalize(camera.uvec);
    camera.vvec = glm::cross(camera.wvec, camera.uvec);
}
