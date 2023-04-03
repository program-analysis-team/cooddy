
struct Base {
    virtual void foo(int* p) = 0;
};

struct Derived1 : public Base {
    virtual void foo(int* p) override {}
};

struct Derived2 : public Base {
    virtual void foo(int* p) override
    {
        if (p != nullptr)
            *p = 0;
    }
};

struct Derived3 : public Base {
    virtual void foo(int* p) override
    {
        *p = 0;
    }
};

void foo(Base* b)
{
    b->foo(nullptr);
}
