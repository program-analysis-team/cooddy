#include <stdio.h>

#include <iostream>
#include <string>

class A {
public:
    A(const std::string& fmt)
    {
        printf(fmt.c_str(), "hello");
    }
};

int main()
{
    std::string user_input;
    std::cin >> user_input;
    A a(user_input);
    return 0;
}
