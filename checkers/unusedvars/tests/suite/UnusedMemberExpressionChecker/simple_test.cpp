
struct A {
    int x = 0;  // no error here
    int y = 0;  // error here
};

int main()
{
    A a;
    a.x = 10;
    a.y = 20;
    return a.x;
}
