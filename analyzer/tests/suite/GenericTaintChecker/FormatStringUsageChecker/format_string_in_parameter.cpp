#include <stdio.h>

#include <iostream>
#include <string>

int out_formatted_string(const std::string& fmt) {
    printf(fmt.c_str(), "hello");
    return 0;
}

int main() {
    std::string user_input;
    std::cin >> user_input;
    out_formatted_string(user_input);
    return 0;
}
