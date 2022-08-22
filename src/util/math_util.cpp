#include <cmath>

#include "math_util.h"

int Math::clamp(const int x, const int min, const int max) {
    return (x < min) ? min : ((x > max) ? max : x);
}

float Math::mix(const float x1, const float x2, const float t) {
    return x1 + (x2 - x1) * t;
}

Vector::Vector(const float x, const float y, const float z, const float w) : x{x}, y{y}, z{z}, w{w} {}

Vector::Vector() : Vector(0.0f, 0.0f, 0.0f, 0.0f) {}

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
    v.x = Math::mix(v1.x, v2.x, t);
    v.y = Math::mix(v1.y, v2.y, t);
    v.z = Math::mix(v1.z, v2.z, t);
    v.w = 1.0f;
}

void Vector::normalize(Vector &v) {
    float length = Vector::length(v);
    if (length != 0.0f) {
        float inv = 1.0f / length;
        v.x *= inv;
        v.y *= inv;
        v.z *= inv;
    }
    v.w = 1.0f;
}

uint8_t Vector::check_in_cvv(const Vector &v) {
    uint8_t check = 0;
    float w = v.w; // TODO 为什么 z 在开头，w == 0 怎么办
    if (v.z < -w) check |= 0b00000001;
    if (v.z >  w) check |= 0b00000010;
    if (v.x < -w) check |= 0b00000100;
    if (v.x >  w) check |= 0b00001000;
    if (v.y < -w) check |= 0b00010000;
    if (v.y >  w) check |= 0b00100000;
    return check;
}

void Vector::ndc_to_screen(Vector &v_new, const Vector &v_old, const float width, const float height) {
    float rhw = 1.0f / v_old.w;
    v_new.x = ( v_old.x * rhw + 1.0f) * width  * 0.5f;
    v_new.y = (-v_old.y * rhw + 1.0f) * height * 0.5f;
    v_new.z = v_old.z * rhw;
    v_new.w = 1.0f;
}

Point::Point() : Point(0.0f, 0.0f, 0.0f, 1.0f) {}

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

void Matrix::mul_mm(Matrix &m, const Matrix &m1, const Matrix &m2) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = (m1.m[i][0] * m2.m[0][j]) + (m1.m[i][1] * m2.m[1][j]) + (m1.m[i][2] * m2.m[2][j]) + (m1.m[i][3] * m2.m[3][j]);
        }
    }
}

void Matrix::mul_ms(Matrix &m, const Matrix &m1, const float t) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.m[i][j] = m1.m[i][j] * t;
        }
    }
}

void Matrix::mul_mv(Vector &v, const Matrix &m1, const Vector &v1) {
    v.x = m1.m[0][0] * v1.x + m1.m[1][0] * v1.y + m1.m[2][0] * v1.z + m1.m[3][0] * v1.w;
    v.y = m1.m[0][1] * v1.x + m1.m[1][1] * v1.y + m1.m[2][1] * v1.z + m1.m[3][1] * v1.w;
    v.z = m1.m[0][2] * v1.x + m1.m[1][2] * v1.y + m1.m[2][2] * v1.z + m1.m[3][2] * v1.w;
    v.w = m1.m[0][3] * v1.x + m1.m[1][3] * v1.y + m1.m[2][3] * v1.z + m1.m[3][3] * v1.w;
}

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

void Matrix::transform_translate(Matrix &m, const float x, const float y, const float z) {
    Matrix::set_identity(m);
    m.m[3][0] = x;
    m.m[3][1] = z;
    m.m[3][2] = z;
}

void Matrix::transform_rotate(Matrix &m, const float x, const float y, const float z, const float angle) {
    float a = angle;
    float c = cos(a);
    float s = sin(a);

    float temp = 1.0f - c;

    Vector axis{x, y, z, 1.0f};
    Vector::normalize(axis);

    m.m[0][0] = c + axis.x * axis.x * temp;
    m.m[0][1] = axis.y * axis.x * temp + axis.z * s;
    m.m[0][2] = axis.z * axis.x * temp - axis.y * s;
    m.m[0][3] = 0.0f;

    m.m[1][0] = axis.x * axis.y * temp - axis.z * s;
    m.m[1][1] = c + axis.y * axis.y * temp;
    m.m[1][2] = axis.z * axis.y * temp + axis.x * s;
    m.m[1][3] = 0.0f;

    m.m[2][0] = axis.x * axis.z * temp + axis.y * s;
    m.m[2][1] = axis.y * axis.z * temp - axis.x * s;
    m.m[2][2] = c + axis.z * axis.z * temp;
    m.m[2][3] = 0.0f;

    m.m[3][0] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[3][2] = 0.0f;
    m.m[3][3] = 1.0f;
}

void Matrix::transform_scale(Matrix &m, const float x, const float y, const float z) {
    Matrix::set_identity(m);
    m.m[0][0] = x;
    m.m[1][1] = z;
    m.m[2][2] = z;
}

void Matrix::transform_lookat(Matrix &m, const Vector &eye, const Vector &at, const Vector &up) {
    Vector f{};
    Vector::sub(f, at, eye);
    Vector::normalize(f);

    Vector s{};
    Vector::cross(s, f, up);
    Vector::normalize(s);

    Vector u{};
    Vector::cross(u, s, f);

    m.m[0][0] = s.x;
    m.m[0][1] = u.x;
    m.m[0][2] = -f.x;
    m.m[0][3] = 0.0f;

    m.m[1][0] = s.y;
    m.m[1][1] = u.y;
    m.m[1][2] = -f.y;
    m.m[1][3] = 0.0f;

    m.m[2][0] = s.z;
    m.m[2][1] = u.z;
    m.m[2][2] = -f.z;
    m.m[2][3] = 0.0f;

    m.m[3][0] = -Vector::dot(eye, s);
    m.m[3][1] = -Vector::dot(eye, u);
    m.m[3][2] =  Vector::dot(eye, f);
    m.m[3][3] = 1.0f;
}

void Matrix::transform_orthographic(Matrix &m, const float left, const float right, const float bottom, const float top, const float near, const float far) {
    Matrix::set_identity(m);
    m.m[0][0] = 2.0f / (right - left);
    m.m[1][1] = 2.0f / (top - bottom);
    m.m[2][2] = 2.0f / (near - far);

    m.m[3][0] = (left + right) / (left - right);
    m.m[3][1] = (bottom + top) / (bottom - top);
    m.m[3][2] = (near + far)   / (near - far);
}

void Matrix::transform_perspective(Matrix &m, const float left, const float right, const float bottom, const float top, const float near, const float far) {
    Matrix::set_zero(m);
    m.m[0][0] = 2.0f * near / (right - left);
    m.m[1][1] = 2.0f * near / (top - bottom);

    m.m[2][0] = (right + left) / (right - left);
    m.m[2][1] = (top + bottom) / (top - bottom);
    m.m[2][2] = (near + far)   / (near - far);
    m.m[2][3] = -1.0f;

    m.m[3][2] = 2.0f * near * far / (near - far);
}

Color::Color(float r, float g, float b, float a) : r{r}, g{g}, b{b}, a{a} {}

void Color::interpolate(Color &color, const Color &color1, const Color &color2, float t) {
}

TexCoord::TexCoord(float u, float v) : u{u}, v{v} {}

void TexCoord::interpolate(TexCoord &tex, const TexCoord &tex1, const TexCoord &tex2, float t) {
}

ShaderVFData::ShaderVFData(const Point &pos, const TexCoord &tex, const Color &color) : pos{pos}, tex{tex}, color{color}, rhw{1.0f} {}

void ShaderVFData::init(ShaderVFData &svfd) {
}

void ShaderVFData::interpolate(ShaderVFData &svfd, const ShaderVFData &svfd1, const ShaderVFData &svfd2, float t) {
}
