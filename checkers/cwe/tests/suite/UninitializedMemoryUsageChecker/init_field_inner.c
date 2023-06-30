struct A {
    int x;
};

struct B {
    struct A a;
};

int main(int argc, char* argv[])
{
    struct B b;
    b.a.x = 42;
    return b.a.x;
}
