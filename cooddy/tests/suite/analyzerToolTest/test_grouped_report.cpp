#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

extern "C" int EXTER_ATTACK foo()
{
    return 1;
}

int* EXTER_ATTACK goo3(int** p)
{
    return *p;
}

int foo(int* v)
{
    int* p = goo3(&v);
    return 0;
}
