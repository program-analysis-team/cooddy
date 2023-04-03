
struct Foo {
    void (*func)(int** p);
} g_foo;

void some(Foo* foo, int** p)
{
    foo->func((p));
}

int main()
{
    int* a = 0;
    some(&g_foo, &(a));
    return *a;
}
