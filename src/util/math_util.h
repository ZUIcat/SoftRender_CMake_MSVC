#pragma once

#include<cstdint>

// C++ 学傻了，暂定如下基本规则
// 复杂类型不作为返回值，先不搞什么移动构造函数乱七八糟的
// 基本类型直接返回，复杂类型作为形参引用传进来，调用者负责创建对象

// -- 常用运算 -- //

class Math {
  public:
    static int clamp(const int x, const int min, const int max);

    // 插值，t 取值 [0, 1]
    static float mix(const float x1, const float x2, const float t);
};


// -- 基本类型 -- //

class Vector {
  public:
    explicit Vector(const float x, const float y, const float z, const float w);
    explicit Vector();
    virtual ~Vector() = default;

  public:
    // 模长
    static float length(const Vector &v);
    // 加法
    static void add(Vector &v, const Vector &v1, const Vector &v2);
    // 减法
    static void sub(Vector &v, const Vector &v1, const Vector &v2);
    // 点乘
    static float dot(const Vector &v1, const Vector &v2);
    // 叉乘
    static void cross(Vector &v, const Vector &v1, const Vector &v2);
    // 插值，t 取值 [0, 1]
    static void interpolate(Vector &v, const Vector &v1, const Vector &v2, float t);
    // 归一化
    static void normalize(Vector &v);

    // 检查坐标是否在视锥体内
    static uint8_t check_in_cvv(const Vector &v);
    // 透视除法后，NDC 到屏幕
    static void ndc_to_screen(Vector &v_new, const Vector &v_old, const float width, const float height);

  public:
    float x;
    float y;
    float z;
    float w;
};

class Point : public Vector {
  public:
    using Vector::Vector;
    explicit Point();
    virtual ~Point() override = default;
};

class Matrix {
  public:
    explicit Matrix(
        const float v11, const float v12, const float v13, const float v14,
        const float v21, const float v22, const float v23, const float v24,
        const float v31, const float v32, const float v33, const float v34,
        const float v41, const float v42, const float v43, const float v44
    );
    explicit Matrix();
    virtual ~Matrix() = default;

  public:
    // 加法
    static void add(Matrix &m, const Matrix &m1, const Matrix &m2);
    // 减法
    static void sub(Matrix &m, const Matrix &m1, const Matrix &m2);
    // 矩阵 * 矩阵
    static void mul_mm(Matrix &m, const Matrix &m1, const Matrix &m2);
    // 矩阵 * 标量
    static void mul_ms(Matrix &m, const Matrix &m1, const float t);
    // 矩阵 * 向量
    static void mul_mv(Vector &v, const Matrix &m1, const Vector &v1);
    // 设置为单位矩阵
    static void set_identity(Matrix &m);
    // 设置为零矩阵
    static void set_zero(Matrix &m);

    // 获取变换矩阵 平移
    static void transform_translate(Matrix &m, const float x, const float y, const float z);
    // 获取变换矩阵 旋转
    static void transform_rotate(Matrix &m, const float x, const float y, const float z, const float angle);
    // 获取变换矩阵 缩放
    static void transform_scale(Matrix &m, const float x, const float y, const float z);
    // 获取变换矩阵 摄像机
    static void transform_lookat(Matrix &m, const Vector &eye, const Vector &at, const Vector &up);
    // 获取变换矩阵 正交投影
    static void transform_orthographic(Matrix &m, const float left, const float right, const float bottom, const float top, const float near, const float far);
    // 获取变换矩阵 透视投影
    static void transform_perspective(Matrix &m, const float left, const float right, const float bottom, const float top, const float near, const float far);

  public:
    float m[4][4]; // 列主序
};

class Color {
  public:
    explicit Color(float r, float g, float b, float a);
    virtual ~Color() = default;

  public:
    static void interpolate(Color &color, const Color &color1, const Color &color2, float t);

  public:
    // [0, 1]
    float r;
    float g;
    float b;
    float a;
};

class TexCoord {
  public:
    explicit TexCoord(float u, float v);
    virtual ~TexCoord() = default;

  public:
    static void interpolate(TexCoord &tex, const TexCoord &tex1, const TexCoord &tex2, float t);

  public:
    // [0, 1]
    float u;
    float v;
};

class ShaderVFData {
  public:
    // 初始的 pos 是未经透视除法的齐次坐标，tex、color 也是原始的，然后经 init() 进行 /w 相关的处理
    explicit ShaderVFData(const Point &pos, const TexCoord &tex, const Color &color);
    virtual ~ShaderVFData() = default;

  public:
    static void init(ShaderVFData &svfd);
    static void interpolate(ShaderVFData &svfd, const ShaderVFData &svfd1, const ShaderVFData &svfd2, float t);

  public:
    Point pos;
    TexCoord tex;
    Color color;
    float rhw;
};
