class Foo {
public:
    Foo()
    {
        myVar = nullptr;
    }

    ~Foo() = default;

    int* myVar;
};

int main()
{
    Foo foo = Foo();
    return *foo.myVar;
}
