#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>

int main(int argc, char* argv[]) {
    char buf[256], fmt[256] = "";
    std::cin >> buf;
    sprintf(fmt, "%d %s", strlen(buf), buf);
    printf(fmt, "Hello!");
    return 0;
}
