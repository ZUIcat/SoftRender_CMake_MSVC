#pragma once

#include<cstdint>

// C++ 学傻了，暂定如下基本规则
// 复杂类型不作为返回值，先不搞什么移动构造函数乱七八糟的
// 基本类型直接返回，复杂类型作为形参引用传进来，调用者负责创建对象

// -- 常用运算 -- //

class Math {
  public:
    static int clamp(int x, int min, int max);

    // 插值，t 取值 [0, 1]
    static float mix(float x1, float x2, float t);
};


// -- 基本类型 -- //

class Vector {
  public:
    explicit Vector(float x, float y, float z, float w);
    explicit Vector();
    virtual ~Vector() = default;

  public:
    // 模长
    static float length(const Vector &v);
    // 加法
    static void add(Vector &v, const Vector &v1, const Vector &v2);
    // 减法
    static void sub(Vector &v, const Vector &v1, const Vector &v2);
    // 矢量点乘
    static float dot(const Vector &v1, const Vector &v2);
    // 矢量叉乘
    static void cross(Vector &v, const Vector &v1, const Vector &v2);
    // 与标量的乘法
    static void mul(Vector &v, const Vector &v1, float t);
    // 插值，t 取值 [0, 1]
    static void interpolate(Vector &v, const Vector &v1, const Vector &v2, float t);
    // 归一化
    static void normalize(Vector &v);

    // 检查坐标是否在视锥体内
    static uint8_t check_in_cvv(const Vector &v);
    // 齐次坐标进行透视除法后，然后从 NDC 到屏幕
    static void homoToScreen(Vector &v_new, const Vector &v_old, float width, float height);

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
        float v11, float v12, float v13, float v14,
        float v21, float v22, float v23, float v24,
        float v31, float v32, float v33, float v34,
        float v41, float v42, float v43, float v44
    );
    explicit Matrix();
    virtual ~Matrix() = default;

  public:
    // 加法
    static void add(Matrix &m, const Matrix &m1, const Matrix &m2);
    // 减法
    static void sub(Matrix &m, const Matrix &m1, const Matrix &m2);
    // 矩阵 * 矩阵
    static void mulMM(Matrix &m, const Matrix &m1, const Matrix &m2);
    // 矩阵 * 标量
    static void mulMS(Matrix &m, const Matrix &m1, float t);
    // 矩阵 * 向量
    static void mulMV(Vector &v, const Matrix &m1, const Vector &v1);
    // 设置为单位矩阵
    static void setIdentity(Matrix &m);
    // 设置为零矩阵
    static void setZero(Matrix &m);

    // 获取变换矩阵 平移
    static void transformTranslate(Matrix &m, float x, float y, float z);
    // 获取变换矩阵 旋转
    static void transformRotate(Matrix &m, float x, float y, float z, float angle);
    // 获取变换矩阵 缩放
    static void transformScale(Matrix &m, float x, float y, float z);
    // 获取变换矩阵 摄像机
    static void transformLookAt(Matrix &m, const Vector &eye, const Vector &at, const Vector &up);
    // 获取变换矩阵 正交投影
    static void transformOrthographic(Matrix &m, float left, float right, float bottom, float top, float near, float far);
    // 获取变换矩阵 透视投影
    static void transformPerspective(Matrix &m, float left, float right, float bottom, float top, float near, float far);

  public:
    float m[4][4]; // 列主序
};

class Color {
  public:
    explicit Color(float r, float g, float b, float a);
    explicit Color();
    virtual ~Color() = default;

  public:
    static void add(Color &color, const Color &color1, const Color &color2);
    static void sub(Color &color, const Color &color1, const Color &color2);
    static void mul(Color &color, const Color &color1, float t);
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
    explicit TexCoord();
    virtual ~TexCoord() = default;

  public:
    static void add(TexCoord &tex, const TexCoord &tex1, const TexCoord &tex2);
    static void sub(TexCoord &tex, const TexCoord &tex1, const TexCoord &tex2);
    static void mul(TexCoord &tex, const TexCoord &tex1, float t);
    static void interpolate(TexCoord &tex, const TexCoord &tex1, const TexCoord &tex2, float t);

  public:
    // [0, 1]
    float u;
    float v;
};

class ShaderVFData {
  public:
    // 初始的 pos 是未经透视除法的齐次坐标，tex、color 也是原始的，然后经 ndc_to_screen() 进行 /w 相关的处理
    explicit ShaderVFData(const Point &pos, const TexCoord &tex, const Color &color);
    explicit ShaderVFData();
    virtual ~ShaderVFData() = default;

  public:
    static void homoToScreen(ShaderVFData &svfd, float width, float height);
    static void add(ShaderVFData &svfd, const ShaderVFData &svfd1, const ShaderVFData &svfd2);
    static void sub(ShaderVFData &svfd, const ShaderVFData &svfd1, const ShaderVFData &svfd2);
    static void interpolate(ShaderVFData &svfd, const ShaderVFData &svfd1, const ShaderVFData &svfd2, float t);
    static void div(ShaderVFData &svfd, const ShaderVFData &svfd1, float t);

  public:
    Point pos;
    TexCoord tex;
    Color color;
    float rhw;
};

class Edge {
  public:
    explicit Edge() = default;
    virtual ~Edge() = default;

  public:
    ShaderVFData svfd1;  // 顶点 1
    ShaderVFData svfd2;  // 顶点 2
    ShaderVFData svfd_i; // 中间插值的临时顶点
};

class ScanLine {
  public:
    explicit ScanLine() = default;
    virtual ~ScanLine() = default;

  public:
    ShaderVFData svfd; // 顶点
    ShaderVFData step; // 步长
    int x;             // 实际的栅格化 x
    int y;             // 实际的栅格化 y
    int width;         // 实际的栅格化 width
};

class Trapezoid {
  public:
    explicit Trapezoid() = default;
    virtual ~Trapezoid() = default;

  public:
    // 注：三角形可视为顶（底）为 0 的梯形
    // 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
    static int initFromTriangle(Trapezoid (&trap)[2], const ShaderVFData &svfd1, const ShaderVFData &svfd2, const ShaderVFData &svfd3);
    // 根据 y 坐标插值，将临时的数据存入 Edge 里的 svfd_i 里
    static void edgeInterpolate(Trapezoid &trap, float y);
    // 根据临时插值数据得到确切的扫描线
    static void getScanLine(ScanLine &scanLine, const Trapezoid &trap, int y);

  public:
    Edge left;    // 左边
    Edge right;   // 右边
    float top;    // 顶长
    float bottom; // 底长
};

