#include <stdio.h>

int main()
{
    FILE* ptr = nullptr;
    auto val = fgetc(ptr);
    return val;
}
