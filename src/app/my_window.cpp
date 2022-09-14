#include "my_window.h"

MyWindow::MyWindow() : Window{u8"软渲染器测试 v1.0", 800, 600} {}

void MyWindow::onCreate(Canvas &canvas){
    canvas.setForeColor(0xFF505DF1);
    canvas.setRenderState(RenderState::COLOR);
};

void MyWindow::onUpdate(Canvas &canvas) {
    // 处理事件
    float k = 0.01f;
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            status = WindowStatus::quit;
            break;
        case SDL_KEYDOWN:
            // 键盘按下事件，可以查看 SDL_KeyboardEvent 结构体内容 event.key.keysym
            switch (event.key.keysym.sym) {
            case SDLK_DOWN:
                x_angle += k * 2;
                break;
            case SDLK_UP:
                x_angle -= k * 2;
                break;
            case SDLK_LEFT:
                y_angle -= k * 2;
                break;
            case SDLK_RIGHT:
                y_angle += k * 2;
                break;
            case SDLK_w:
                distance -= k;
                break;
            case SDLK_s:
                distance += k;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            // 鼠标按钮按下事件，可以查看 SDL_MouseButtonEvent 结构体内容 event.button.x, event.button.y
            break;
        default:
            break;
        }
    }
    SDL_Log("x_angle: %f\ty_angle: %f\tdistance: %f", x_angle, y_angle, distance);

    // 清除颜色缓冲和深度缓冲
    canvas.clear(0xFF88C9FF, 2.0f);

    // canvas.drawLine(10, 10, 100, 100, canvas.getForeColor());

    // 准备顶点数据
    Point point[] {
        Point{ -1, -1,  1 },
        Point{  1, -1,  1 },
        Point{  1,  1,  1 },
        Point{ -1,  1,  1 },
        Point{ -1, -1, -1 },
        Point{  1, -1, -1 },
        Point{  1,  1, -1 },
        Point{ -1,  1, -1 },
    };
    // 准备索引数据
    int index[] {
        0, 1, 2, 0, 2, 3, // 正面
        5, 4, 7, 5, 7, 6, // 背面
        4, 0, 3, 4, 3, 7, // 左侧
        1, 5, 6, 1, 6, 2, // 右侧
        3, 2, 6, 3, 6, 7, // 上侧
        4, 5, 1, 4, 1, 0, // 下侧
    };
    // 准备 UV 数据
    float tex[] {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    // 准备颜色数据
    float color[] {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    // 获取 MVP 矩阵
    Matrix m{}, mt{}, mr_yx{}, mr_x{}, mr_y{};
    Matrix::transformTranslate(mt, 0.0f, 0.0f, distance);
    Matrix::transformRotate(mr_x, 1.0f, 0.0f, 0.0f, x_angle);
    Matrix::transformRotate(mr_y, 0.0f, 1.0f, 0.0f, y_angle);
    //
    Matrix ms{};
    Matrix::transformScale(ms, 0.25f, 0.25f, 0.25f);
    Matrix::mulMM(mr_x, mr_x, ms);
    //
    Matrix::mulMM(mr_yx, mr_y, mr_x);
    Matrix::mulMM(m, mt, mr_yx); // TODO 变换顺序问题
    Matrix mvp{}, vm{}, v{}, p{};
    Matrix::transformLookAt(v, Vector{0.0f, 0.0f, 0.0f}, Vector{0.0f, 0.0f, -1.0f}, Vector{0.0f, 1.0f, 0.0f});
    float ratio = static_cast<float>(canvas.getWidth()) / static_cast<float>(canvas.getHeight());
    //float near_k = 1.0f;
    //Matrix::transformOrthographic(p, -ratio * near_k, ratio * near_k, -1.0f * near_k, 1.0f * near_k, 0.1f, 1000.0f);
    float near_k = 0.1f;
    Matrix::transformPerspective(p, -ratio * near_k, ratio * near_k, -1.0f * near_k, 1.0f * near_k, 0.1f, 1000.0f);
    Matrix::mulMM(vm, v, m);
    Matrix::mulMM(mvp, p, vm);
    // MVP * Pos -> Homo Pos
    ShaderVFData svfds[8];
    for (int i = 0; i < 8; i++) {
        Point tempP{};
        Matrix::mulMV(tempP, mvp, point[i]);
        svfds[i] = ShaderVFData {
            tempP,
            TexCoord{tex[2 * i], tex[2 * i + 1]},
            Color{color[4 * i], color[4 * i + 1], color[4 * i + 2], color[4 * i + 3]}
        };
    }
    // Check in CVV -> Screen Pos -> 绘制
    canvas.drawPrimitives(svfds, sizeof(svfds) / sizeof(*svfds), index, sizeof(index) / sizeof(*index));
};

void MyWindow::onDestroy(Canvas &canvas){
};
