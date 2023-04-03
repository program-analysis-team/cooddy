#include "base.h"

struct Derived1 : public Base {
    virtual void foo(int* p) override;
};

struct Derived2 : public Derived1 {};
