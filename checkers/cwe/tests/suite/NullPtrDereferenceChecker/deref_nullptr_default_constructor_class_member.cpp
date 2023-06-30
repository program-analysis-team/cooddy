class Foo {
public:
    Foo(int* value = nullptr)
    {
        myVar = value;
    }

    ~Foo() = default;

    int GetDerefVar()
    {
        return *myVar;
    }

private:
    int* myVar;
};

int main()
{
    Foo foo = Foo();
    return foo.GetDerefVar();
}
