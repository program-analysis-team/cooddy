
struct S1 {
    virtual int foo() = 0;
};

struct S {
    virtual S1* foo() = 0;
};

int boo1(S* s)
{
    if (s->foo() == 0) {
        return 0;
    }
    return s->foo()->foo();  // no error here
}
