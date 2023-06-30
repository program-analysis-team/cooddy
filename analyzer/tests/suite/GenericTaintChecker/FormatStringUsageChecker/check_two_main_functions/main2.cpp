#include <stdio.h>

#include <iostream>
#include <string>

void func();

int main()
{
    func();
    return 0;
}

void func()
{
    std::string s;
    std::cin >> s;
    const char* hello = "Hello";
    printf(s.c_str(), hello);
}
