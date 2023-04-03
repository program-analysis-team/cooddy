#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

int main(int argc, char* argv[]) {
    std::string buf;
    char fmt[256];
    std::cin >> buf;
    buf.copy(fmt,sizeof fmt - 1);
    printf(fmt, "Hello!");
    return 0;
}
