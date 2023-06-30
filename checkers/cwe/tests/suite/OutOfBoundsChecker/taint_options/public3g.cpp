// No error: called from this translation unit, not public api
int f(int a)
{
    int arr[10] = {};

    return arr[a];
}

int main()
{
    return f(0);
}
