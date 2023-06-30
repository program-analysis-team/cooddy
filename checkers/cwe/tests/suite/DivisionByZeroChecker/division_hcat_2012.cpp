
#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

EXTER_ATTACK void foo(int a, int b)
{
    if (a == 0 || b == 0)
        return;

    int x = a / b;   // abs(b)>abs(a), result will be 0
    auto z = a / x;  // DBZ
}
