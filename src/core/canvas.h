#pragma once

#include <cstdint>

#include "render.h"

// 绘制种类
enum class RenderState {
    WIREFRAME = 0b00000001, // 纯线框
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
    virtual void clear(uint32_t color);
    // Base 2D, no depth
    virtual void drawPoint(int x, int y, uint32_t color);
    virtual void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
    // For 3D, with depth
    virtual void drawScanLine(const ScanLine &scanLine);
    virtual void drawTrapezoid(const Trapezoid &trapezoid);
    virtual void drawPrimitive(const ShaderVFData &svfd1, const ShaderVFData &svfd2, const ShaderVFData &svfd3);

  public:
    inline virtual int getWidth() const { return width; };
    inline virtual int getHeight() const { return height; };
    inline virtual uint32_t *const getColorBuffer() const { return colorBuffer; };
    inline virtual uint8_t *const getDepthBuffer() const { return depthBuffer; };
    inline virtual RenderState getRenderState() const { return renderState; };
    inline virtual void setRenderState(RenderState renderState) { this->renderState = renderState; };

  private:
    int width;                                        // 宽度
    int height;                                       // 高度
    uint32_t *colorBuffer = nullptr;                  // 颜色缓冲附件
    uint8_t *depthBuffer = nullptr;                   // 深度缓冲附件
    RenderState renderState = RenderState::WIREFRAME; // 现在的绘制种类
};
