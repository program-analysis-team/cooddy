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
    printf(s.c_str(), "Hello");
}
