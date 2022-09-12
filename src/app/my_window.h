#pragma once

#include "../core/window.h"

class MyWindow : public Window {
  public:
    explicit MyWindow();
    virtual ~MyWindow() = default;

  public:
    virtual void onCreate(Canvas &canvas) override;
    virtual void onUpdate(Canvas &canvas) override;
    virtual void onDestroy(Canvas &canvas) override;

  private:
    float x_angle = 0.0f;
    float y_angle = 0.0f;
    float distance = -1.5f;
};
