class A {
public:
    A()
    {
        a = 0;
        b = 0;
    }
    int a;
    int b;
};

int main()
{
    A a;
    int b = a.a + 3;
}
