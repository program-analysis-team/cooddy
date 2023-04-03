// Error: using arg without checking in an untrusted function
#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

static int EXTER_ATTACK f(int a)
{
    int arr[10] = {};

    return arr[a];
}
