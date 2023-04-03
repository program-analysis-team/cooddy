
struct A {
};

A* Init() {
    return new A();
}

A* GetInstance()
{
    static A* a = Init();
    return a;
}

void foo()
{
    GetInstance();  // no error
}
