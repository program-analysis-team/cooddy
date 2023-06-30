
struct A {
    int *p1;
};

struct A *bar();
void baz(struct A *a){free(a->p1);};

int foo()
{
    int *p1 = 0;
    p1 = (int *)malloc(42);
    if (p1 == 0) {
        return -1;
    }
    struct A *a = bar();
    a->p1 = p1;
    baz(a);
    return 0;
}
