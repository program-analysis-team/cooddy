

int main()
{
    int arr[10] = {0};
    int idx = 0;
    idx += 5;
    idx *= 2;
    int c = arr[idx - 1];  // no error here
    return arr[idx];       // error here
}
