
struct A {
};

int GetInstance()
{
    static A* a = new A();
    return 0;
}
