
int main()
{
    int arr[10] = {0};
    int v = arr[9];  // no error here
    return arr[11];  // out of bounds
}
