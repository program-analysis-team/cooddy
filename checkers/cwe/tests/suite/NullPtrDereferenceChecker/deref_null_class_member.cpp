class Foo {
public:
    Foo()
    {
        myVar = 0;
    }

    ~Foo() = default;

    int* myVar;
};

int main()
{
    Foo foo = Foo();
    return *foo.myVar;
}
