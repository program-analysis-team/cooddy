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

    int GetAnotherVar()
    {
        return myAnotherValue;
    }

private:
    int* myVar;
    int myAnotherValue = 5;
};

int main()
{
    Foo foo = Foo();
    if (foo.GetAnotherVar() < 10) {
        return *foo.GetVar();
    }
    return 0;
}
