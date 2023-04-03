
class A {
public:
    static A* GetInstance()
    {
        if (!a) {
            a = new A();
        }
        return a;
    }

private:
    static A* a;
};

A* A::a = new A();

void foo()
{
    A::GetInstance();  // no error
}
