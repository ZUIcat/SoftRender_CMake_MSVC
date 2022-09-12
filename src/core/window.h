#pragma once

#include <cstdint>
#include <string>

#include <SDL.h>

#include "canvas.h"

enum class WindowStatus {
    loop,
    pause,
    quit
};

class Window {
  public:
    explicit Window(std::string title, int width, int height);
    virtual ~Window();

  public:
    virtual bool init();
    virtual void startLoop();
    virtual void endLoop();
    virtual void destroy();

  public:
    virtual void onCreate(Canvas &canvas);
    virtual void onUpdate(Canvas &canvas);
    virtual void onDestroy(Canvas &canvas);

  public:
    inline virtual int getWidth() const { return width; };
    inline virtual int getHeight() const { return height; };
    inline virtual const std::string getTitle() const { return title; };
    inline virtual const Canvas &getCanvas() const { return canvas; };

  protected:
    int width;
    int height;
    WindowStatus status = WindowStatus::loop;
    std::string title;
    Canvas canvas;

  protected:
    SDL_Window *pWindow = nullptr;
    SDL_Renderer *pRenderer = nullptr;
    SDL_Texture *pTexture = nullptr;
};
