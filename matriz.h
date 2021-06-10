//
// Created by henry on 3/06/2021.
//

#ifndef CG_4_ESCENA_MATRIZ_H
#define CG_4_ESCENA_MATRIZ_H


class Matriz {

public:
    float mat[16];
    Matriz() {
        LoadIndentity();
    }

    void LoadIndentity();
    void scale(float x, float y, float z);
    void rotacion(float x, float y, float z);
    void setRotationXMatrix(float theta);
    void setRotationYMatrix(float theta);
    void setRotationZMatrix(float theta);
    void traslacion(float x, float y, float z);

    void Multiplicacion(const Matriz &m);
    Matriz operator*(const Matriz &m);


    // View Matrix
    void SetCamera(float posX, float posY, float posZ,
                   float lookAtX, float lookAtY, float lookAtZ,
                   float upx, float upy, float upz);

    //  Proyection Matrix
    void ProjectionMatrix(float _fov, float _ratio, float _near, float _far);

    void normalize(float *a);
    void crossProduct( float *a, float *b, float *res);
};


#endif //CG_4_ESCENA_MATRIZ_H
