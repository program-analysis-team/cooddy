class B {
    int field;
};

class A {
public:
    A() : ptr(new B()) {}
    ~A()
    {
        delete ptr;
    }
    B* ptr;
};

int main()
{
    A val;
    return 0;
}
