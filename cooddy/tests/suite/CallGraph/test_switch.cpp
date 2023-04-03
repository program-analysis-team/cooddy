#include <memory.h>

void call1() {}
void call2() {}
void call3() {}
void call4() {}
void call5() {}

void switchCall(int a)
{
    switch (a) {
        case 1:
            call1();
            break;
        case 2:
            call2();
        case 3:
            call3();
            break;
        default:
            call5();
        case 4:
            call4();
            break;
    }
}

void switchCall1()
{
    switchCall(1);
}

void switchCall2()
{
    switchCall(2);
}

void switchCall3()
{
    switchCall(3);
}

void switchCall4()
{
    switchCall(4);
}

void switchCall5()
{
    switchCall(5);
}

int main()
{
    switchCall1();
    switchCall2();
    switchCall3();
    switchCall4();
    switchCall5();
    return 0;
}
