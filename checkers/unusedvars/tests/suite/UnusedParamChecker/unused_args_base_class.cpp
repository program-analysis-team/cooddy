class A {
public:
    A() = default;
    virtual int foo(int a)
    {
        return a;
    };
};

class B : public A {
public:
    B() = default;
    int foo(int a) override
    {
        return 123;
    }
};
