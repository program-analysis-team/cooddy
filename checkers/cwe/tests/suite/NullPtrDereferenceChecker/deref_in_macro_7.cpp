#define BAR(x, b, c) x + *(b->a->fptr) + c

struct A {
    A* a;
    int* fptr;
};

void bar(A* aa)
{
    aa->a->fptr = nullptr;
    BAR(0, aa, 1);
}
