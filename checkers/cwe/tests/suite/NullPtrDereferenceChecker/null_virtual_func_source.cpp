
struct S1 {
    virtual int foo() = 0;
};

struct S {
    virtual S1* foo() = 0;  // source here
};

int boo1(S* s)
{
    return s->foo()->foo();
}
