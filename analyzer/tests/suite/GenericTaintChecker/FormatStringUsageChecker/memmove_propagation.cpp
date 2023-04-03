#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    char buf[256], fmt[256] = "";
    std::cin >> buf;
    memmove(fmt, buf, strlen(buf) + 1);
    printf(fmt, "Hello!");
    return 0;
}
