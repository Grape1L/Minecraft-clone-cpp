#pragma once

#include <cstring>
#include <OpenGL/Math/Vec3.h>
#include <cmath>
#include <OpenGL/MiscStuff.h>

class Mat4 {
public:
    Mat4() {
        setIdentity();
    }

    void setIdentity() {
        ::memset(mat, 0, sizeof(float) * 16);
        mat[0][0] = 1.0f;
        mat[1][1] = 1.0f;
        mat[2][2] = 1.0f;
        mat[3][3] = 1.0f;
    }

    void setScale(const Vec3 &scale) {
        mat[0][0] = scale.x;
        mat[1][1] = scale.y;
        mat[2][2] = scale.z;
    }

    void setTranslation(const Vec3 &translation) {
        mat[3][0] = translation.x;
        mat[3][1] = translation.y;
        mat[3][2] = translation.z;
    }

    void operator *= (const Mat4 &matrix) {
        Mat4 out;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                out.mat[i][j] = 
                    mat[i][0] * matrix.mat[0][j] + 
                    mat[i][1] * matrix.mat[1][j] + 
                    mat[i][2] * matrix.mat[2][j] + 
                    mat[i][3] * matrix.mat[3][j];
            }
        }
        *this = out;
    }

    void setRotationX(float x) {
        double radX = calcRAD(x);
        mat[1][1] = cos(radX);
        mat[1][2] = sin(radX);
        mat[2][1] = -sin(radX);
        mat[2][2] = cos(radX);
    }

    void setRotationY(float y) {
        double radY = calcRAD(y);
        mat[0][0] = cos(radY);
        mat[0][2] = -sin(radY);
        mat[2][0] = sin(radY);
        mat[2][2] = cos(radY);
    }

    void setRotationZ(float z) {
        double radZ = calcRAD(z);
        mat[0][0] = cos(radZ);
        mat[0][1] = sin(radZ);
        mat[1][0] = -sin(radZ);
        mat[1][1] = cos(radZ);
    }

    void setOrthoLH(float width, float height, float nearPlane, float farPlane) {
        mat[0][0] = 2.0f / width;
        mat[1][1] = 2.0f / height;
        mat[2][2] = 1.0f / (farPlane - nearPlane);
        mat[3][2] = -nearPlane / (farPlane - nearPlane);
    }

    void setPerspectiveRH(float fovY, float aspect, float nearPlane, float farPlane) {
        ::memset(mat, 0, sizeof(mat));
        float f = 1.0f / tanf(fovY / 2);

        mat[0][0] = f / aspect;
        mat[1][1] = f;
        mat[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        mat[2][3] = -1.0f;
        mat[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    }

public:
    float mat[4][4] = {};
};