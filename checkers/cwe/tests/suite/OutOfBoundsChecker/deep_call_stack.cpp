
void foo1(int x)
{
    int arr[10] = {0};
    arr[x];
}

void foo2(int x)
{
    foo1(x);
}

void foo3(int x)
{
    foo2(x);
}

void foo4(int x)
{
    foo3(x);
}

void foo5(int x)
{
    foo4(x);
}

void foo6(int x)
{
    foo5(x);
}

void foo7(int x)
{
    foo6(x);
}

void foo8(int x)
{
    foo7(x);
}

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo(int x)
{
    foo8(x);
}
