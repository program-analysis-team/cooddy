#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

template<typename T, typename U>
struct A {
};

template<>
struct A<int, int> {
    int* EXTER_ATTACK goo3(int** p)
    {
        return *p;
    }
};
void foo() {
    A<int, int> a;
    int **p = nullptr;
    a.goo3(p);
}
