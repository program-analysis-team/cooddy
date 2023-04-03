struct Bar {
    int* ptr;
};

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK foo2(Bar* bar)
{
    if (bar != nullptr) {
        return *bar->ptr;  // error here
    }
    return 0;
}
