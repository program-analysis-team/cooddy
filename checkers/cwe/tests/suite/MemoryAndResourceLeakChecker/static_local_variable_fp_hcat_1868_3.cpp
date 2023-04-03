
struct A {
};

A* GetInstance()
{
    return new A();
}

void foo()
{
    GetInstance();  // error
}
