#include <stdio.h>

#include <iostream>
#include <string>

std::string get_format_string() {
    std::string result;
    std::cin >> result;
    return result;
}

void copy_arg(const char*& arg1, const char* arg2) { arg1 = arg2; }

const char* return_arg(const char* arg) { return arg; }

int main() {
    std::string s = get_format_string();
    const char* fmt;
    copy_arg(fmt, return_arg(s.c_str()));
    printf(fmt, 10);
    return 0;
}
