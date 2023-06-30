struct A {
    int val;
    int* pointer;
};

int main()
{
    A a = {0};
    return *a.pointer;
}
