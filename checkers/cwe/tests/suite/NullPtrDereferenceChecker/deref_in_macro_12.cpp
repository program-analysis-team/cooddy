struct AAA {
    int* Foo(int)
    {
        int x = 42;
        return nullptr;
    }
};

struct BBB {
    AAA* aaaPtr;
};

#define BAR(b, qq) *(b.aaaPtr)

void foo1(BBB bbbObj)
{
    int qq;
    bbbObj.aaaPtr = nullptr;
    BAR(bbbObj, qq);
}
