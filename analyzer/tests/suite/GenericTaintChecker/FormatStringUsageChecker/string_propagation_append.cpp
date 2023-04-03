#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>

int main(int argc, char* argv[]) {
    std::string buf, fmt = "";
    std::cin >> buf;
    fmt.append(buf);
    printf(fmt.c_str(), "Hello!");
    return 0;
}
