
struct A {
    int* func(int* p)
    {
        return 0;
    }

    template <class T>
    T func1(int* p)
    {
        T* a = func(p);
        if (a == 0 || *a == 1) {
            return 1;
        }
        return *a;
    }
};

int main()
{
    A a;
    a.func1<int>(0);
    return 0;
}
