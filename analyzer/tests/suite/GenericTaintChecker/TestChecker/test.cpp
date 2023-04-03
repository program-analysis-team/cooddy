#include <iostream>

int ReadInput()
{
    int var = 0;
    std::cin >> var;
    return var;
}

void FilterInput(int& var)
{
    // ...
}

void UseInput(int var)
{
    // ...
}

int main()
{
    int var1 = ReadInput();
    FilterInput(var1);
    UseInput(var1);  // no problem here

    int var2 = ReadInput();
    UseInput(var2);  // problem here
    return 0;
}
