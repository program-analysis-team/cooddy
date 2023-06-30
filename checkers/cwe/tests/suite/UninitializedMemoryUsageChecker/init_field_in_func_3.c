struct A {
    int x;
};

void foo(struct A** ptrptr)
{
    struct A* ptr = *ptrptr;
    ptr->x = 42;
}

int main()
{
    struct A a;
    struct A* ptr = &a;
    struct A** ptrptr = &ptr;
    foo(ptrptr);
    return a.x;
}
