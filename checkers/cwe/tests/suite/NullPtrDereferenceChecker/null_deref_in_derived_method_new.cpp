
struct Base {
    virtual void foo(int* p) = 0;
};

struct Derived1 : public Base {
    virtual void foo(int* p) override
    {
        *p = 0;
    }
};

void foo(Base* b, int* p)
{
    b->foo(p);
}

void bar()
{
    auto d = new Derived1();
    foo(d, nullptr);  // no error
}
