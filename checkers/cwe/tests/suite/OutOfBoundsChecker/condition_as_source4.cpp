
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))

struct S1 {
    int arr[11];
    unsigned int count;
};

struct S2 {
    unsigned int count;
};

void bar(S1* s, unsigned int v)
{
    s->arr[v] = v;
}

void foo(S1* s, S1* s2)
{
    int v = MIN(s2->count - 3, 8 - 1);
    bar(s, s2->count);
}
