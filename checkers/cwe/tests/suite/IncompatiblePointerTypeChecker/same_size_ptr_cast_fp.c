#include <stdint.h>

void foo(uint8_t *ptr){};

int main()
{
    char buffer = (char)0;
    foo((uint8_t *)&buffer);
    return 0;
}
