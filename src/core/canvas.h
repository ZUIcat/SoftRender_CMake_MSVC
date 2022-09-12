#pragma once

#include <cstdint>

#include "render.h"

// 绘制种类
enum class RenderState {
    WIREFRAME = 0b00000001, // 线框
    TEXTURE   = 0b00000010, // 材质采样
    COLOR     = 0b00000100, // 颜色插值
};

// 抽象的画布类
// TODO 设计的不好，不应该依赖于 render.h 里的东西
class Canvas {
  public:
    explicit Canvas(int width, int height);
    virtual ~Canvas();

  public:
    // 清除 颜色缓冲附件 和 深度缓冲附件
    virtual void clear(uint32_t color, float depth);
    // 设置 获取 前景色
    inline virtual void setForeColor(uint32_t color) { foreColor = color; }
    inline virtual uint32_t getForeColor() { return foreColor; }

    // == Base 2D, no depth ==
    // 画点（屏幕空间）
    virtual void drawPoint(int x, int y, uint32_t color);
    // 画线（屏幕空间）
    virtual void drawLine(int x1, int y1, int x2, int y2, uint32_t color);

    // == For 3D, with depth ==
    // 画扫描线（屏幕空间）
    virtual void drawScanLine(const ScanLine &scanLine);
    // 画梯形（屏幕空间）
    virtual void drawTrapezoid(const Trapezoid &trapezoid);
    // 画三角形（屏幕空间）
    virtual void drawPrimitive(const ShaderVFData &svfd1, const ShaderVFData &svfd2, const ShaderVFData &svfd3);
    // 画三角形们（齐次空间，会自动进行 homoToScreen）
    virtual void drawPrimitives(const ShaderVFData *const svfd_p, size_t svdf_len, const int *const index_p, size_t index_len);

  public:
    inline virtual int getWidth() const { return width; };
    inline virtual int getHeight() const { return height; };
    inline virtual uint32_t *const getColorBuffer() const { return colorBuffer; };
    inline virtual float *const getDepthBuffer() const { return depthBuffer; };
    inline virtual RenderState getRenderState() const { return renderState; };
    inline virtual void setRenderState(RenderState renderState) { this->renderState = renderState; };

  private:
    int width;                                        // 宽度
    int height;                                       // 高度
    uint32_t *colorBuffer = nullptr;                  // 颜色缓冲附件（ARGB8888）
    float *depthBuffer = nullptr;                     // 深度缓冲附件
    RenderState renderState = RenderState::WIREFRAME; // 现在的绘制种类
    uint32_t foreColor;                               // 前景色（主要用于线框）
};
