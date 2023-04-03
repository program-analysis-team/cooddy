#include "derived.h"

struct Derived3 : public Derived2 {
    virtual void foo(int* p) override {}
};
