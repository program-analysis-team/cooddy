class Foo {
public:
    Foo() {}

    ~Foo() = default;

    int* GetVar()
    {
        return myVar;
    }

private:
    int* myVar = nullptr;
};

int main()
{
    Foo foo = Foo();
    return *foo.GetVar();
}
