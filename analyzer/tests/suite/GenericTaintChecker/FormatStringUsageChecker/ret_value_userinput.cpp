#include <stdio.h>

#include <iostream>
#include <string>

std::string get_format_string() {
    std::string result;
    std::cin >> result;
    return result;
}

int main() {
    printf(get_format_string().c_str(), 10);
    return 0;
}
