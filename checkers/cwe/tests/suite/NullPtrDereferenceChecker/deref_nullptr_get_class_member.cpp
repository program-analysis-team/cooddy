class Foo {
public:
    Foo()
    {
        myVar = nullptr;
    }

    ~Foo() = default;

    int* GetVar()
    {
        return myVar;
    }

private:
    int* myVar;
};

int main()
{
    Foo foo = Foo();
    return *foo.GetVar();
}
