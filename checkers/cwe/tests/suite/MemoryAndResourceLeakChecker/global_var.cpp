#include <cstdlib>

int* ptr = 0;

void foo() {
    ptr = (int*)malloc(42);
}
