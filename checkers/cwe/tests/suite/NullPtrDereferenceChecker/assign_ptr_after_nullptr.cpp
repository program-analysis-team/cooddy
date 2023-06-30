#include <stdlib.h>

int main()
{
    int* a = nullptr;
    a = (int*)malloc(123);
    if (a != 0) {
        int b = *a;
    }
    return 0;
}
