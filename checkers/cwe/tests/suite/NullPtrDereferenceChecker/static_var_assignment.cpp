
void* (*allocate_foo)(unsigned);

void* my_malloc(unsigned sz)
{
    if (allocate_foo != nullptr) {
        return allocate_foo(sz);
    }
    return nullptr;
}

class AAA {
    static AAA* inst;

public:
    void foo() {}
    static AAA* Inst()
    {
        if (inst == nullptr) {
            inst = (AAA*)my_malloc(42);
        }
        *inst;
        return inst;
    }
};
