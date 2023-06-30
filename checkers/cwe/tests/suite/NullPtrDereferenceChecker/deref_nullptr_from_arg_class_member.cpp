class Foo {
public:
    Foo(int* value)
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
    Foo foo = Foo(nullptr);
    return foo.GetDerefVar();
}
