#include <stdio.h>

#include <iostream>
#include <string>

int main() {
    std::string user_iput, fmt;
    std::cin >> user_iput;
    fmt = user_iput;
    printf(fmt.c_str(), 10);
    return 0;
}
