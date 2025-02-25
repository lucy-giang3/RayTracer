// Readfile definitions 
#ifndef READFILE_H
#define READFILE_H

#include <GL/glut.h>

void matransform (stack<mat4> &transfstack, GLfloat * values) ;
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, GLfloat * values) ;
string readfile (const char * filename) ;
#endif