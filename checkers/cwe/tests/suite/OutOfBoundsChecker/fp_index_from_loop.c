#include <stdint.h>

void Foo() {
    uint32_t arr[2] = {0};

    for (uint32_t index = 0; index < 10; ++index) {
        arr[0]++;
    }
}
