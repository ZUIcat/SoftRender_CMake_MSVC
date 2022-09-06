#include "canvas.h"

Canvas::Canvas(int width, int height) : width{width}, height{height} {
    colorBuffer = new uint32_t[width * height];
    depthBuffer = new uint8_t[width * height];
}

Canvas::~Canvas() {
    delete[] colorBuffer;
    delete[] depthBuffer;
}

void Canvas::clear(uint32_t color) {
    for (int i = 0; i < width * height; i++) {
        colorBuffer[i] = color;
        depthBuffer[i] = color;
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
    
}

void Canvas::drawTrapezoid(const Trapezoid &trapezoid) {

}

void Canvas::drawPrimitive(const ShaderVFData &svfd1, const ShaderVFData &svfd2, const ShaderVFData &svfd3) {

}
