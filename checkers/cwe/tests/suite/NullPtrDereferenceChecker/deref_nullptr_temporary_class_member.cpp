class Foo {
public:
    Foo()
    {
        myVar = nullptr;
    }

    int* myVar;
};

int Bar(Foo f)
{
    return *f.myVar;
}

int main()
{
    Bar(Foo());
    return 0;
}
