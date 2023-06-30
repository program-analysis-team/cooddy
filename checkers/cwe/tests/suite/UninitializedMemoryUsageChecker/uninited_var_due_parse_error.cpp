
struct C;

struct A {
    C* ff();
};

struct B : public A {
    int p;
};

int boo(A* a)
{
    B* b = dynamic_cast<B*>(a->ff());
    return b->p; // no error here
}
