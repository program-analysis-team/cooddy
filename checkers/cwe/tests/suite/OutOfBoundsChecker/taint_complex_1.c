// Source: manually annotated function
// Propagation: assignment
// Path sensitivity: condition on the path
// Sink: array index

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

void EXTER_ATTACK f(int x, int cond)
{
    int arr[10] = {};
    int y = x;

    if (cond) {
        arr[x] = 0;
    }
}
