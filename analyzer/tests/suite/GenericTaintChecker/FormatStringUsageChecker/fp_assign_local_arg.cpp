#include <stdio.h>

#include <iostream>
#include <string>

void get_format_string(std::string arg) { std::cin >> arg; }

int main() {
    std::string fmt = "Hello";
    get_format_string(fmt);
    printf(fmt.c_str(), 10);
    return 0;
}
