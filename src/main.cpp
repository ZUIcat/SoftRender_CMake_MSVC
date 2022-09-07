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

    class MyStruct {
      //public:
      //  explicit MyStruct() = default;
      //  virtual ~MyStruct() = default;

      private:
        float top;    // 顶长
        float bottom; // 底长
    };

    MyStruct m;
    MyStruct m2{};

    // https://stackoverflow.com/questions/9515327/default-constructors-initialization-of-pod-and-implicit-type-conversions-in-c
    // https://stackoverflow.com/questions/2417065/does-the-default-constructor-initialize-built-in-types

    MyWindow window;
    window.init();
    window.startLoop();

    std::cout << "[APP START]" << std::endl;
    return 0;
}
