#include "window.h"

Window::Window(std::string title, int width, int height) : title{title}, width{width}, height{height}, canvas{width, height} {}

Window::~Window() {
    destroy();
}

bool Window::init() {
    int sdl_ret = SDL_Init(SDL_INIT_VIDEO);
    if (sdl_ret != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError());
        return false;
    }

    pWindow = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0
    );
    if (pWindow == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError());
        return false;
    }

    pRenderer = SDL_CreateRenderer(
        pWindow,
        -1,
        SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC // TODO
    );
    if (pRenderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError());
        return false;
    }

    pTexture = SDL_CreateTexture(
        pRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );
    if (pRenderer == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", SDL_GetError());
        return false;
    }

    onCreate(canvas);

    return true;
}

void Window::startLoop() {
    status = WindowStatus::loop;

    uint64_t nFrequency, nPrevCounter, nCurrCounter, nElapsedCounter;
    float elapsed = 0.0f, totalTime = 0.0f;
    int fps = 0, fpsCount = 0;
    nFrequency = SDL_GetPerformanceFrequency();
    nPrevCounter = SDL_GetPerformanceCounter();

    while (status != WindowStatus::quit) {
        // 状态为 pause 就跳过这次循环
        if (status == WindowStatus::pause) {
            continue;
        }

        // 计算 FPS
        nCurrCounter = SDL_GetPerformanceCounter();
        nElapsedCounter = nCurrCounter - nPrevCounter;
        nPrevCounter = nCurrCounter;
        // 前后两帧的耗时(ms)
        elapsed = (nElapsedCounter * 1000.0f) / nFrequency;
        totalTime += elapsed;
        // 已经过了 1 秒
        if (totalTime > 1000.0f) {
            totalTime -= 1000.0f;
            fps = fpsCount;
            fpsCount = 1;
            SDL_Log("FPS: %d", fps);
        } else {
            fpsCount++;
        }

        // 清除背景
        // SDL_SetRenderDrawColor(pRenderer, 50, 50, 50, 255);
        // SDL_RenderClear(pRenderer);

        onUpdate(canvas);

        // 渲染图像
        SDL_UpdateTexture(pTexture, NULL, canvas.getColorBuffer(), canvas.getWidth() * sizeof(uint32_t));
        SDL_RenderCopy(pRenderer, pTexture, NULL, NULL);

        // 显示图像
        SDL_RenderPresent(pRenderer);
    }
}

void Window::endLoop() {
    status = WindowStatus::quit;
}

void Window::destroy() {
    onDestroy(canvas);

    if (pTexture) {
        SDL_DestroyTexture(pTexture);
    }
    if (pRenderer) {
        SDL_DestroyRenderer(pRenderer);
    }
    if (pWindow) {
        SDL_DestroyWindow(pWindow);
    }

    SDL_Quit();
}

void Window::onCreate(Canvas &canvas) {}

void Window::onUpdate(Canvas &canvas) {}

void Window::onDestroy(Canvas &canvas) {}
