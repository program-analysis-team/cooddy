struct S {
    int v;
    S* n;
};

#define GET_PTR(s) ((s == 0 || s->v) ? nullptr : s->n)

int foo2(S* s)
{
    auto p = GET_PTR(GET_PTR(GET_PTR(s)));
    return 0;
}

int foo(S* s)
{
    if (s && s->n && (s ? s->n : nullptr)->v == 11) {
        auto p = (s ? s->n : nullptr)->v == 22 ? nullptr : s->n->n;
        *p;
    }
    return 0;
}
