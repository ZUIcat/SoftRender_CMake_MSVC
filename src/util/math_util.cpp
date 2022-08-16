#include <cmath>

#include "math_util.h"

int CMID(const int x, const int min, const int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

int interpolate(const float x1, const float x2, const float t) {
    return x1 + (x2 - x1) * t;
}

Vector::Vector(const float x, const float y, const float z, const float w) : x{x}, y{y}, z{z}, w{w} {}

Vector::Vector() : Vector(0.0f, 0.0f, 0.0f, 0.0f) {}

Vector::~Vector() {}

float Vector::length(const Vector &v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void Vector::add(Vector &v, const Vector &v1, const Vector &v2) {
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    v.z = v1.z + v2.z;
    v.w = 1.0f;
}

void Vector::sub(Vector &v, const Vector &v1, const Vector &v2) {
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    v.z = v1.z - v2.z;
    v.w = 1.0f;
}

float Vector::dot(const Vector &v1, const Vector &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void Vector::cross(Vector &v, const Vector &v1, const Vector &v2) {
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    v.w = 1.0f;
}

void Vector::interpolate(Vector &v, const Vector &v1, const Vector &v2, float t) {
    v.x = ::interpolate(v1.x, v2.x, t);
    v.y = ::interpolate(v1.y, v2.y, t);
    v.z = ::interpolate(v1.z, v2.z, t);
    v.w = 1.0f;
}

void Vector::normalize(Vector &v) {
    float length = Vector::length(v);
    if (length != 0.0f) {
        float inv = 1.0f / length; // TODO 这样有什么用意吗
        v.x *= inv;
        v.y *= inv;
        v.z *= inv;
    }
}

Point::Point() : Point(0.0f, 0.0f, 0.0f, 1.0f) {}

Point::~Point() {}

Matrix::Matrix(
    const float v11, const float v12, const float v13, const float v14,
    const float v21, const float v22, const float v23, const float v24,
    const float v31, const float v32, const float v33, const float v34,
    const float v41, const float v42, const float v43, const float v44
) : m {
    {v11, v12, v13, v14},
    {v21, v22, v23, v24},
    {v31, v32, v33, v34},
    {v41, v42, v43, v44}
} {}

Matrix::Matrix() : Matrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
) {}

Matrix::~Matrix() {}

void Matrix::add(Matrix &m, const Matrix &m1, const Matrix &m2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = m1.m[i][j] + m2.m[i][j];
        }
    }
}

void Matrix::sub(Matrix &m, const Matrix &m1, const Matrix &m2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = m1.m[i][j] - m2.m[i][j];
        }
    }
}

void Matrix::mul(Matrix &m, const Matrix &m1, const Matrix &m2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = (m1.m[i][0] * m2.m[0][j]) + (m1.m[i][1] * m2.m[1][j]) + (m1.m[i][2] * m2.m[2][j]) + (m1.m[i][3] * m2.m[3][j]);
        }
    }
}

void Matrix::scale(Matrix &m, const Matrix &m1, const float t) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = m1.m[i][j] * t;
        }
    }
}

void Matrix::apply(Matrix &m, const Matrix &m1, const Vector &v1) {}

void Matrix::set_identity(Matrix &m) {
    m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
    m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
    m.m[1][0] = m.m[1][2] = m.m[1][3] = 0.0f;
    m.m[2][0] = m.m[2][1] = m.m[2][3] = 0.0f;
    m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
}

void Matrix::set_zero(Matrix &m) {
    m.m[0][0] = m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
    m.m[1][0] = m.m[1][1] = m.m[1][2] = m.m[1][3] = 0.0f;
    m.m[2][0] = m.m[2][1] = m.m[2][2] = m.m[2][3] = 0.0f;
    m.m[3][0] = m.m[3][1] = m.m[3][2] = m.m[3][3] = 0.0f;
}
