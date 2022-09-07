#include "my_window.h"

MyWindow::MyWindow() : Window{u8"软渲染器测试 v1.0", 1280, 720} {}

void MyWindow::onCreate(Canvas &canvas){
};

void MyWindow::onUpdate(Canvas &canvas) {
    canvas.clear(0xFF88C9FF, 0.0f);
    canvas.drawLine(10, 10, 100, 100, 0);
};

void MyWindow::onDestroy(Canvas &canvas){
};
