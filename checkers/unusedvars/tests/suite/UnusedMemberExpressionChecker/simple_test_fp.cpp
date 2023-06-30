
struct A {
    int x = 0;  // no error here
    int y = 0;  // error here
};

union B {
    int x;
    int y;
};

int main()
{
    A a;
    B b;
    return a.x + a.y;
}
