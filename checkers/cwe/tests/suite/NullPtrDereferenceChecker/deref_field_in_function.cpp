
struct A {
    int* m_f;
};

int func(A* a)
{
    return *a->m_f;
}

int main()
{
    A* a = new A();
    a->m_f = 0;
    return func(a);
}
