#include "main.h"
#include "./app/my_window.h"

int main(int argc, char *argv[]) {
    std::cout << "[APP START]" << std::endl;

    MyWindow window;
    window.init();
    window.startLoop();

    std::cout << "[APP END]" << std::endl;
    return 0;
}
