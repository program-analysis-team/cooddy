#include <memory.h>

void call1() {}
void call2() {}
void call3() {}
void call4() {}

bool switchCall(int a, int b)
{
    int v = 0;
    if (a == 3) {
        if (b == 4) {
            v = 1;
        } else {
            v = 2;
        }
    } else {
        if (b == 4) {
            v = 3;
        } else {
            v = 4;
        }
    }

    if (v == 1) {
        call1();
    } else if (v == 2) {
        call2();
    } else if (v == 3) {
        call3();
    } else if (v == 4) {
        call4();
    }
}

void switchCall1()
{
    switchCall(3, 4);
}

void switchCall2()
{
    switchCall(3, 5);
}

void switchCall3()
{
    switchCall(4, 4);
}

void switchCall4()
{
    switchCall(4, 5);
}

int main()
{
    switchCall1();
    switchCall2();
    switchCall3();
    switchCall4();
    return 0;
}
