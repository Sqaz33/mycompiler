#include "iostream"

extern "C" void __sl_print(int x) {
    std::cout << x << std::endl;
}

extern "C" int __sl_input() {
    int in;
    std::cin >> in;
    return in;
}

extern "C" void __sl_start();

int main() {
    __sl_start();
}