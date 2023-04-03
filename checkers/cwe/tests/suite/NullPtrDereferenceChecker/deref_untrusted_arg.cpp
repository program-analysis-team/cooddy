struct Bar {
    int* ptr;
};

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK foo(Bar* bar)
{
    return *bar->ptr;  // error here
}
