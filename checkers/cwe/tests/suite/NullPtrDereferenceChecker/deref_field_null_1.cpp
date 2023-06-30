
struct A {
    int *m0, *m1, *m2;
    int* m3[5];
};

int main()
{
    A *a, *b;
    a->m2 = 0;
    b = a;
    return *b->m3[2];
}
