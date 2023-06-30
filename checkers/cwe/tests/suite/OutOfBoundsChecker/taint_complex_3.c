// Source: return value of an unknown function
// Propagation: call argument
// Path sensitivity: conditional operator
// Sink: array index

int taint();

int g(int x, int cond)
{
    int arr[10] = {};

    return cond ? arr[x] : 0;
}

void f(int cond)
{
    g(taint(), cond);
}
