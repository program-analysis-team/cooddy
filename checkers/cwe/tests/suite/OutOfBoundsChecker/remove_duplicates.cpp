
void boo(int p)
{
    int arr[20];
    if (p > 100) {
        return;
    }
    arr[p] = p;
    arr[p] = p + 1;
    arr[p] = p + 2;
}
