
struct S {
    int* p;
};

int foo(S* s, int v)
{
    if (v == 10 && s->p == nullptr) {
        return *s->p;  // NPD here
    }
    return 0;
}
