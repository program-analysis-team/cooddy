
struct S {
    int* p;
};

int foo(S* s, int v)
{
    int ret = *s->p;
    if (v == 10 && s->p == nullptr) {
        return ret;
    }
    return 0;
}
