struct A {
    int x;
    int y = 1;
};

int main()
{
    A a;
    int b = a.y;  // no error here
    return a.x;   // error here
}
