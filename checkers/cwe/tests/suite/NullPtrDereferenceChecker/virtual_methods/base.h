
struct Base {
    virtual void foo(int* p) = 0;
};

extern void foo(Base* b, int* p);
