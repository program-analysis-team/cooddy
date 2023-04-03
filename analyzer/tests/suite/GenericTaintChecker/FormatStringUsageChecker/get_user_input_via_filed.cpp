#include <stdio.h>

#include <iostream>
#include <string>

struct Foo {
    std::string myField;
    std::string myField2;
};

void readUserInput(Foo& foo)
{
    std::cin >> foo.myField;
}

int main()
{
    Foo foo;
    readUserInput(foo);
    printf(foo.myField2.c_str(), 13);
    printf(foo.myField.c_str(), 10);
    return 0;
}
