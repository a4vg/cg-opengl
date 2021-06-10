//
// Created by henry on 3/06/2021.
//
#include <cmath>
#include <memory>
#include "matriz.h"

// res = a cross b;
void Matriz::crossProduct( float *a, float *b, float *res) {
    res[0] = a[1] * b[2]  -  b[1] * a[2];
    res[1] = a[2] * b[0]  -  b[2] * a[0];
    res[2] = a[0] * b[1]  -  b[0] * a[1];
}

// Normalize a vec3
void Matriz::normalize(float *a) {
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    a[0] /= mag;
    a[1] /= mag;
    a[2] /= mag;
}


void Matriz::LoadIndentity() {
    for (int i=0; i < 16; i++) {
        mat[i] = 0.0;
    }
    mat[0]=mat[5]=mat[10]=mat[15]=1.0;
}
void Matriz::scale(float x, float y, float z) {
    LoadIndentity();
    mat[0] = x;
    mat[5] = y;
    mat[10] = z;
}
void Matriz::rotacion(float x, float y, float z) {
    LoadIndentity();
    mat[0] = cos(x); mat[1] = sin(x);
    mat[4] = -sin(x); mat[5] = cos(x);

    Matriz ry, rz;

    *this = *this * ry * rz;

}
void Matriz::setRotationXMatrix(float theta) {
    LoadIndentity();
    mat[5] = cos(theta);
    mat[6] = sin(theta);
    mat[9] = -sin(theta);
    mat[10] = cos(theta);;
}

// Define a matrix of rotation in Y
void Matriz::setRotationYMatrix(float theta) {
    LoadIndentity();
    mat[0] = cos(theta);
    mat[2] = -sin(theta);
    mat[8] = sin(theta);
    mat[10] = cos(theta);;
}

// Define a matrix of rotation in Z
void Matriz::setRotationZMatrix(float theta) {
    LoadIndentity();
    mat[0] = cos(theta);
    mat[1] = sin(theta);
    mat[4] = -sin(theta);
    mat[5] = cos(theta);;
}



void Matriz::traslacion(float x, float y, float z) {
    LoadIndentity();
    mat[3] = x;
    mat[7] = y;
    mat[11] = z;
}

void Matriz::ProjectionMatrix(float _fov, float _ratio, float _near, float _far) {

    float f = 1.0f / tan(_fov * (M_PI / 360.0));

    LoadIndentity();

    mat[0] = f / _ratio;
    mat[1 * 4 + 1] = f;
    mat[2 * 4 + 2] = (_far + _near ) / (_near - _far);
    mat[3 * 4 + 2] = (2.0f * _far * _near) / (_near - _far);
    mat[2 * 4 + 3] = -1.0f;
    mat[3 * 4 + 3] = 0.0f;
}

void Matriz::SetCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ,
                       float upx, float upy, float upz) {
    float dir[3], right[3], up[3];
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;

    dir[0] =  (lookAtX - posX);
    dir[1] =  (lookAtY - posY);
    dir[2] =  (lookAtZ - posZ);
    normalize(dir);

    crossProduct(dir,up,right);
    normalize(right);

    crossProduct(right,dir,up);
    normalize(up);

    //float aux[16];

    mat[0]  = right[0];
    mat[4]  = right[1];
    mat[8]  = right[2];
    mat[12] = 0.0f;

    mat[1]  = up[0];
    mat[5]  = up[1];
    mat[9]  = up[2];
    mat[13] = 0.0f;

    mat[2]  = -dir[0];
    mat[6]  = -dir[1];
    mat[10] = -dir[2];
    mat[14] =  0.0f;

    mat[3]  = 0.0f;
    mat[7]  = 0.0f;
    mat[11] = 0.0f;
    mat[15] = 1.0f;

    Matriz aux;
    aux.traslacion(-posX, -posY, -posZ);
    Multiplicacion(aux);
}

// a = a * b;
void Matriz::Multiplicacion(const Matriz &m) {
    float res[16];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                res[j*4 + i] += mat[k*4 + i] * m.mat[j*4 + k];
            }
        }
    }
    //memcpy(mat, res, 16 * sizeof(float));
    for (int i = 0; i < 16; i++) {
        mat[i] = res[i];
    }
}


Matriz Matriz::operator*(const Matriz &m) {

}

