
struct A {
};

A* GetInstance()
{
    static A* a = new A();
    A* b = a;
    return b;
}

void foo()
{
    GetInstance();  // no error
}
