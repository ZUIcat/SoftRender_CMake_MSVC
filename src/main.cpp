#include "main.h"
#include "./app/my_window.h"

struct MyStruct {
    int a;
    float b;
    MyStruct() = default;
    //MyStruct(){};
};

int main(int argc, char *argv[]) {
    std::cout << "[APP END]" << std::endl;

    MyWindow window;
    window.init();
    window.startLoop();

    std::cout << "[APP START]" << std::endl;
    return 0;
}
