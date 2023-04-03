
struct A {
    int* m_f;
};

int main()
{
    A *a, *b;
    a->m_f = 0;
    b = a;
    return *b->m_f;
}
