
#define DEREF(a) *(a)

#define FIELD(a, f) (a->f)

#define DEREF_FIELD(a, f) DEREF(FIELD(a, f))

#define ZERO 0

#define COMPARE_FIELD(a, f) (DEREF_FIELD(a, f) == ZERO)

struct S {
    int* f;
};

void foo(S* s)
{
    S a;
    s->f = 0;
    ZERO;
    DEREF(s->f);
    FIELD(s, f);
    DEREF_FIELD(s, f);
    COMPARE_FIELD(s, f);
}
