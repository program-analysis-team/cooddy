
struct S {
    S* s;
    int arr[10];
};

void foo(S* s)
{
    int arr[10];
    auto ss = s->s->s->s->s->s->s->s->s->s;
    arr[ss->arr[0]] = 0;
}
