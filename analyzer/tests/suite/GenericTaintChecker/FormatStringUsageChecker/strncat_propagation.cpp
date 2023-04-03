#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    char buf[256], fmt[256] = "";
    std::cin >> buf;
    strncat(fmt, buf, strlen(buf));
    printf(fmt, "Hello!");
    return 0;
}
