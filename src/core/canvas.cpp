#include "canvas.h"

Canvas::Canvas(int width, int height) : width{width}, height{height} {
    colorBuffer = new uint32_t[width * height];
    depthBuffer = new float[width * height]; // TODO 初始值为 0 可以吗？
}

Canvas::~Canvas() {
    delete[] colorBuffer;
    delete[] depthBuffer;
}

void Canvas::clear(uint32_t color, float depth) {
    for (int i = 0; i < width * height; i++) {
        colorBuffer[i] = color;
        depthBuffer[i] = depth;
    }
}

void Canvas::drawPoint(int x, int y, uint32_t color) {
    colorBuffer[x + width * y] = color;
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    int nowX = x1;
    int nowY = y1;

    int delta = 0;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int k1 = dx << 1;
    int k2 = dy << 1;
    int xStep = 1;
    int yStep = 1;

    if (dx < 0) {
        dx = -dx;
        k1 = -k1;
        xStep = -xStep;
    }

    if (dy < 0) {
        dy = -dy;
        k2 = -k2;
        yStep = -yStep;
    }

    drawPoint(nowX, nowY, color);
    if (dx < dy) {
        while (nowY != y2) {
            nowY += yStep;
            delta += k1;
            if (delta >= dy) {
                nowX += xStep;
                dy += k2;
            }
            drawPoint(nowX, nowY, color);
        }
    } else {
        while (nowX != x2) {
            nowX += xStep;
            delta += k2;
            if (delta >= dx) {
                nowY += yStep;
                dx += k1;
            }
            drawPoint(nowX, nowY, color);
        }
    }
}

void Canvas::drawScanLine(const ScanLine &scanLine) {
    int line_x_now = scanLine.x;
    int line_witdh_now = scanLine.width;
    int canvas_width = width;

    uint32_t *thisColorBuffer = &colorBuffer[canvas_width * scanLine.y];
    float *thisDepthBuffer = &depthBuffer[canvas_width * scanLine.y];

    ShaderVFData v = scanLine.svfd;
    for (; line_witdh_now > 0; line_x_now++, line_witdh_now--) { // TODO 边界条件
        if (line_x_now >= 0 && line_x_now < canvas_width) {
            float rhw = v.rhw; // 获取 1 / w（即 1 / -z）
            if (rhw > thisDepthBuffer[line_x_now]) { // 深度测试
                thisDepthBuffer[line_x_now] = rhw; // 更新深度缓存
                float w = 1.0f / rhw; // 获取 w（即 -z）
                // 颜色插值模式
                if (static_cast<int>(renderState) & static_cast<int>(RenderState::COLOR)) {
                    int real_r = Math::clamp(static_cast<int>(v.color.r * w * 255), 0, 255);
                    int real_g = Math::clamp(static_cast<int>(v.color.g * w * 255), 0, 255);
                    int real_b = Math::clamp(static_cast<int>(v.color.b * w * 255), 0, 255);
                    int real_a = Math::clamp(static_cast<int>(v.color.a * w * 255), 0, 255);
                    thisColorBuffer[line_x_now] = (real_a << 24) | (real_r << 16) | (real_g << 8) | (real_b);
                }
                // 材质采样模式
                if (static_cast<int>(renderState) & static_cast<int>(RenderState::TEXTURE)) {
                    // TODO
                }
            }
        }
        //else {
        //    break;
        //}
        ShaderVFData::add(v, v, scanLine.step);
    }
}

void Canvas::drawTrapezoid(const Trapezoid &trapezoid) {
    int canvas_height = height;
    int top = static_cast<int>(trapezoid.top + 0.5f);
    int bottom = static_cast<int>(trapezoid.bottom + 0.5f);

    ScanLine s{}; // TODO no copy
    Trapezoid t = trapezoid; // TODO no copy

    for (int i = top; i < bottom; i++) { // TODO 边界条件
        if (i >= 0 && i < canvas_height) {
            Trapezoid::getScanLine(s, t, i);
            drawScanLine(s);
        }
        //else {
        //    break;
        //}
    }
}

void Canvas::drawPrimitive(const ShaderVFData &svfd1, const ShaderVFData &svfd2, const ShaderVFData &svfd3) {

}
