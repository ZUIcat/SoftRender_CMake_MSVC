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
            // 获取深度值
            float depth = v.pos.z;
            if (depth < thisDepthBuffer[line_x_now]) { // 深度测试
                thisDepthBuffer[line_x_now] = depth;   // 更新深度缓存
                float w = 1.0f / v.rhw;
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
            // 光栅化，得到扫描线
            Trapezoid::getScanLine(s, t, i);
            // 绘制扫描线
            // TODO 在这个方法之前，或者在这个方法之中可以加入 frag shader 阶段
            drawScanLine(s);
        }
        //else {
        //    break;
        //}
    }
}

void Canvas::drawPrimitive(const ShaderVFData &svfd1, const ShaderVFData &svfd2, const ShaderVFData &svfd3) {
    Trapezoid trapezoid[2]; // TODO no create

    // 颜色插值模式 或 材质采样模式
    if (static_cast<int>(renderState) & (static_cast<int>(RenderState::COLOR) | static_cast<int>(RenderState::TEXTURE))) {
        int n = Trapezoid::initFromTriangle(trapezoid, svfd1, svfd2, svfd3);
        if (n >= 1) Canvas::drawTrapezoid(trapezoid[0]);
        if (n == 2) Canvas::drawTrapezoid(trapezoid[1]);
    }
    // 线框模式
    // TODO 这里偷懒了 线框模式也需要深度检测的
    if (static_cast<int>(renderState) & static_cast<int>(RenderState::WIREFRAME)) {
        // TODO 直接 static_cast<int>() 吗？不四舍五入或者算一下？
        Canvas::drawLine(static_cast<int>(svfd1.pos.x), static_cast<int>(svfd1.pos.y), static_cast<int>(svfd2.pos.x), static_cast<int>(svfd2.pos.y), foreColor);
        Canvas::drawLine(static_cast<int>(svfd2.pos.x), static_cast<int>(svfd2.pos.y), static_cast<int>(svfd3.pos.x), static_cast<int>(svfd3.pos.y), foreColor);
        Canvas::drawLine(static_cast<int>(svfd3.pos.x), static_cast<int>(svfd3.pos.y), static_cast<int>(svfd1.pos.x), static_cast<int>(svfd1.pos.y), foreColor);
    }
}

void Canvas::drawPrimitives(const ShaderVFData *const svfd_p, size_t svdf_len, const int *const index_p, size_t index_len) {
    for (size_t i = 0; i < index_len; i += 3) {
        int svfd_index_1 = index_p[i];
        int svfd_index_2 = index_p[i + 1];
        int svfd_index_3 = index_p[i + 2];
        if (svfd_index_1 >= 0 && svfd_index_1 < svdf_len &&
            svfd_index_2 >= 0 && svfd_index_2 < svdf_len &&
            svfd_index_3 >= 0 && svfd_index_3 < svdf_len) {
            ShaderVFData sdvf_1{svfd_p[svfd_index_1]};
            ShaderVFData sdvf_2{svfd_p[svfd_index_2]};
            ShaderVFData sdvf_3{svfd_p[svfd_index_3]};
            // 这里是在画三角形的时候裁剪的，当三角形每个顶点都符合才通过，然后再 /w
            if (Vector::checkInCVV(sdvf_1.pos) == 0 &&
                Vector::checkInCVV(sdvf_2.pos) == 0 &&
                Vector::checkInCVV(sdvf_3.pos) == 0) {
                ShaderVFData::homoToScreen(sdvf_1, width, height);
                ShaderVFData::homoToScreen(sdvf_2, width, height);
                ShaderVFData::homoToScreen(sdvf_3, width, height);
                // TODO 其实也可以在光栅化的时候裁剪，考虑加上 Guard-band 算法
                drawPrimitive(sdvf_1, sdvf_2, sdvf_3);
            }
        }
    }
}
