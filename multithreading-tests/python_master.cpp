#include <iostream>
#include "submodule.hpp"

Submodule submodule(10);

extern "C" float testFromPy(int val) {
    return 2.5 * submodule.test(val);
}

int main(int argc, char *argv[]) {
    std::cout << "main function called" << std::endl;
    return 0;
}
