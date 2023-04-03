#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    char buf[256], fmt[256] = "";
    std::cin >> buf;
    strcpy(fmt, buf);
    printf(fmt, "Hello!");
    return 0;
}
