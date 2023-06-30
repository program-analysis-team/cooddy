
struct Foo {
    int method();
};

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK boo(Foo& foo)
{
    int arr[10] = {0};
    return arr[foo.method()];
}
