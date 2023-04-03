// Error: using value returned by an undefined function
int taint();

int f()
{
    int arr[10] = {};

    int x = 0;
    x = taint();

    return arr[x];
}
