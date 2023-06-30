
typedef struct _S {
    int field;
} S;

void func1(S* s)
{
    s->field = 0;
}

void func3(S* s)
{
    func1(s);
}

void func4(S* s)
{
    func3(s);
}

int func5(S* s, int x)
{
    if (x == 10) {
        return 0;
    }
    func4(s);
    return 1;
}

int foo(int x)
{
    S s;
    if (func5(&s, x) == 1) {
        return s.field;
    }
    return 0;
}
