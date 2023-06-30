struct A {
    int a;
    char* b;
    int c[10];
};

void foo(struct A* a)
{
    int x = a->a;
}

int main(int argc, char* argv[])
{
    struct A a;
    foo(&a);
    return 0;
}
