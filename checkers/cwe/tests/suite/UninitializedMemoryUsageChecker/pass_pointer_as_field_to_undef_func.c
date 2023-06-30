
struct B {
    int val;
};

struct A {
    void* ptr;
};

void bar(struct A* a);

int foo(int x)
{
    struct B b;
    struct A a;
    a.ptr = &b;
    if (x == 0) {
        bar(&a);
        return b.val;
    }
    return 0;
}
