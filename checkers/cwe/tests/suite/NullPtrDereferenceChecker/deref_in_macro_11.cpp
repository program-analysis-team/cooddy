struct A {
    int* Aaa()
    {
        return nullptr;
    }
};

struct B {
    A* Bbb(A* a)
    {
        return a;
    }
};

#define BOO(aobj, bobj) *(bobj->Bbb(aobj)->Aaa());

void bar(A* a, B* b)
{
    BOO(a, b);
}
