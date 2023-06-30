struct AAA {
    int* Foo(int)
    {
        int x = 42;
        return nullptr;
    }
};

struct BBB {
    AAA aaaObj;
};

#define BAR(b, qq) *(b->aaaObj.Foo(qq))

void foo1(BBB* bbbPtr)
{
    int qq;
    BAR(bbbPtr, qq);
}
