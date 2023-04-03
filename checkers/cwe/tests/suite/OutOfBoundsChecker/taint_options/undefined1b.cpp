// Error: using arg transformed by an undefined function
void taint(int& x);

int f()
{
    int arr[10] = {};

    int x = 0;
    taint(x);

    return arr[x];
}
