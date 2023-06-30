
struct Base {
    virtual void foo(int* p) = 0;
};

struct Derived1 : public Base {
    virtual void foo(int* p) override
    {
        *p = 0;
    }
};

struct Derived2 : public Derived1 {};

struct Derived3 : public Derived2 {
    virtual void foo(int* p) override {}
};

void foo(Base* b, int* p)
{
    b->foo(p);
}

void bar()
{
    Derived3 d3;
    foo(&d3, nullptr);  // no error

    Derived2 d2;
    foo(&d2, nullptr);  // error here
}
