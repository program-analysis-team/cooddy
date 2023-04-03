#include <stdio.h>

#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    char buf[256], fmt[256], *p1 = nullptr, *p2 = nullptr;
    std::cin >> buf;
    for (p1 = buf, p2 = fmt; *p1 != 0; p1++, p2++) {
        *p2 = *p1;
    }
    *p2 = 0;
    printf(fmt, "Hello!");
    return 0;
}
