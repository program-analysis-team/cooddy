#include "test.h"

void MyFunc2()
{
    func4();
}

void func1()
{
    MyFunc2();
}

int main()
{
    MyFunc2();
    func1();
    return 0;
}
