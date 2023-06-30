struct A {
    int a;
    char* b;
};

void init(struct A* a)
{
    a->a = 0;
    a->b = 0;
}

int main(int argc, char* argv[])
{
    struct A a;
    init(&a);
    return a.a;
}
