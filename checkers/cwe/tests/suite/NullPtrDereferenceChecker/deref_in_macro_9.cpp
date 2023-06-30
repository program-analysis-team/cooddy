struct AAA {
    int* Foo(int)
    {
        int x = 42;
        return nullptr;
    }
    int* Foo()
    {
        int x = 42;
        return nullptr;
    }
};

struct BBB {
    AAA* aaaPtr;
};

#define BAR1(b, qq) *(b->aaaPtr->Foo(qq))
#define BAR2(b) *(b->aaaPtr->Foo())

void foo1(BBB* bbbPtr)
{
    int qq;
    BAR1(bbbPtr, qq);
}

void foo2(BBB* bbbPtr)
{
    int qq;
    BAR2(bbbPtr);
}
