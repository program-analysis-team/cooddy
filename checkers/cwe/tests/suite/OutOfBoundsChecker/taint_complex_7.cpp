// Source: manually annotated function
// Propagation: loop index
// Path sensitivity: conditional operator
// Sink: array index

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

static void EXTER_ATTACK f(int x, bool cond)
{
    int arr[10] = {};

    for (int i = x; i < x + 5; i++) {
        arr[cond ? 0 : i] = 0;
    }
}
