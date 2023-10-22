#include "submodule.hpp"

Submodule::Submodule() {
    val = 0;
}

Submodule::Submodule(int val) {
    this->val = val;
}

int Submodule::test(int arg) {
    int tmp = val + arg;
    val = arg;
    return tmp;
}