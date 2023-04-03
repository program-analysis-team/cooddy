#define SIZE 10

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK foo1(int i)
{
    int arr[SIZE];
    if (i < SIZE) {
        arr[i];  // error here
    }
}

void EXTER_ATTACK foo2(int i)
{
    int arr[SIZE];
    if (i >= 0 && i < SIZE) {
        arr[i];  // no error here
    }
}

void EXTER_ATTACK foo3(unsigned i)
{
    int arr[SIZE];
    if (i < SIZE) {
        arr[i];  // no error here
    }
}
