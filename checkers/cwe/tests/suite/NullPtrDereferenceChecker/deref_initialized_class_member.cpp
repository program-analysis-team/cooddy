class Foo {
public:
    void func()
    {
        myVar = 0;
    }
    int* myVar;
};

int main()
{
    Foo foo = Foo();
    foo.func();
    return *foo.myVar;
}
