// Transform.cpp: implementation of the Transform class.

#include "Transform.h"

// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
    // initialize 3x3 matrix to be returned
    mat3 ret(1);

    // convert to radians
    float radians = glm::radians(degrees);

    // calculate rotation in radians
    float cosine = cosf(radians);
    float sine = sinf(radians);

    // normalize axis to ensure unit vector
    vec3 norm = glm::normalize(axis);

    // calculate angle between input axis and vector being rotated
    float magicNum = 1.0f;
    float scalingFactor = (magicNum - cosine);
    vec3 scaled = scalingFactor * norm;

    // update matrix elements

    // row 1
    ret[0][0] = cosine + scaled.x * norm.x;
    ret[0][1] = scaled.x * norm.y + sine * norm.z;
    ret[0][2] = scaled.x * norm.z - sine * norm.y;
    // row 2
    ret[1][0] = scaled.y * norm.x - sine * norm.z;
    ret[1][1] = cosine + scaled.y * norm.y;
    ret[1][2] = scaled.y * norm.z + sine * norm.x;
    // row 3
    ret[2][0] = scaled.z * norm.x + sine * norm.y;
    ret[2][1] = scaled.z * norm.y - sine * norm.x;
    ret[2][2] = cosine + scaled.z * norm.z;

    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
    // rotate eye vector around up vector by given degrees
    // transform the eye(?) vector using the rotation matrix
    eye = rotate(degrees, up) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
    // use cross product to get rotation matrix
    // normalize to ensure unit vector first
    mat3 mat = rotate(degrees, glm::normalize(glm::cross(eye, up)));

    // apply the rotation matrix to the eye/up vector
    eye = mat * eye;
    up = mat * up;
}

mat4 Transform::lookAt(const vec3& eye, const vec3& center, const vec3& up)
{
    mat4 ret;
    // calculate w
    vec3 w = glm::normalize(eye);

    // calculate u
    vec3 crossVectorU = glm::cross(up, eye);
    vec3 u = glm::normalize(crossVectorU);

    // calculate v
    vec3 crossVectorV = glm::cross(w, u);
    vec3 v = glm::normalize(crossVectorV);

    // calculate the dot product
    float dotU = glm::dot(u, -eye);
    float dotV = glm::dot(v, -eye);
    float dotW = glm::dot(w, -eye);

    // create the view matrix
    vec4 row1(u.x, u.y, u.z, dotU);
    vec4 row2(v.x, v.y, v.z, dotV);
    vec4 row3(w.x, w.y, w.z, dotW);
    vec4 row4(0, 0, 0, 1);

    mat4 view(row1, row2, row3, row4);

    // transpose the view matrix
    ret = glm::transpose(view);

    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    // convert fovy to radians
    float radians = glm::radians(fovy);

    // calculate cotangent of fov angle
    float tangent = tanf(radians / 2.0f);
    float cotangent = 1.0f / tangent;

    // set up magic numbers
    float transf_constant = 1.0f;
    float constant_range = 2.0f;

    // construct perspective matrix
    mat4 perspective(0.0f);
    perspective[0][0] = cotangent / aspect;
    perspective[1][1] = cotangent;
    perspective[2][2] = -(zFar + zNear) / (zFar - zNear);
    perspective[2][3] = -(transf_constant);
    perspective[3][2] = -(constant_range * zFar * zNear) / (zFar - zNear);

    /*
     testing

     perspective[2][2] = (zFar + zNear) / (zFar - zNear);
     perspective[2][3] = -(transf_constant);
     perspective[3][2] = (constant_range * zFar * zNear) / (zFar - zNear);

    */

    return perspective;
}

mat4 Transform::scale(const float& sx, const float& sy, const float& sz)
{
    mat4 ret(1.0f);   // 4x4 matrix to be returned

    // adjust matrix according to given values
    ret[0][0] = sx;
    ret[1][1] = sy;
    ret[2][2] = sz;

    return ret;
}

mat4 Transform::translate(const float& tx, const float& ty, const float& tz)
{
    mat4 ret(1.0f);    // 4x4 matrix to be returned

    // adjust matrix according to given values
    ret[3][0] = tx;
    ret[3][1] = ty;
    ret[3][2] = tz;

    return ret;
}


vec3 Transform::upvector(const vec3& up, const vec3& zvec)
{
    vec3 x = glm::cross(up, zvec);
    vec3 y = glm::cross(zvec, x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
