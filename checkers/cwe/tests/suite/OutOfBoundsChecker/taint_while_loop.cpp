
struct Foo {
    int size;
};

int arr[10];

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK bar(const Foo* foo)
{
    int i = 0;
    int to = foo->size;
    while (i < to) {
        arr[i++] = 0;
    }
    return 0;
}
