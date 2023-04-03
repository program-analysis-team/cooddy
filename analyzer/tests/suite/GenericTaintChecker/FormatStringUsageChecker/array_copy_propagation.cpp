#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    char buf[256], fmt[256] = "", *p = nullptr;
    std::cin >> buf;
    for(p = buf; *p != 0; p++) {
        fmt[p - buf] = *p;
    }
    fmt[p - buf] = 0;
    printf(fmt, "Hello!");
    return 0;
}
