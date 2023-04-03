
struct Base {
    virtual void foo(int* p) {}
};

struct Derived : public Base {
    virtual void foo(int* p) override
    {
        *p = 0;
    }
};

void foo(Base* b)
{
    b->Base::foo(nullptr);  // no error here
}
