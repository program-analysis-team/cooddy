
struct S {
    int* p;
};

void foo5(void* s)
{
    static int a = 10;
    ((S*)s)->p = &a;
}

void foo4(void* s)
{
    foo5(s);
}

void foo3(void* s)
{
    foo4(s);
}

void foo2(S* s, int** p)
{
    foo3(s);
    *p = s->p;
}

void foo1()
{
    S s = {0};
    int* p = 0;
    foo2(&s, &p);
    *p = 10;
}
