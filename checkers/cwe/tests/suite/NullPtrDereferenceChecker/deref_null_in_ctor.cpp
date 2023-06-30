class A {
public:
    int myX;
    A(int* x)
    {
        myX = *x;
    }
};

int main()
{
    A a(nullptr);
}
