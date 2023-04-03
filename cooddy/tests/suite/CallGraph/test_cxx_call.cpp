
class Foo {
public:
    Foo(int param)
    {
        myParam = param;
    }

    void call1() {}

    void call2() {}

    void call3() {}

    void call4() {}

    void paramCall(int a)
    {
        if (a == 1) {
            if (myParam == 3) {
                call1();
            } else {
                call2();
            }
        } else {
            if (myParam == 3) {
                call3();
            } else {
                call4();
            }
        }
    }

private:
    int myParam;
};

void paramCall1()
{
    Foo(3).paramCall(1);
}

void paramCall2()
{
    Foo foo(4);
    foo.paramCall(1);
}

void paramCall3()
{
    Foo foo(3);
    Foo bar = foo;
    bar.paramCall(2);
}

void paramCall4()
{
    Foo foo(4);
    Foo bar(3);
    bar = foo;
    bar.paramCall(2);
}

int main()
{
    paramCall1();
    paramCall2();
    paramCall3();
    paramCall4();
    return 0;
}
