
class A {
public:
    static A* a;
};

A* A::a = new A();

void foo()
{
    A* b = A::a;
}
