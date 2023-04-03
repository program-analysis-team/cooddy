

int main()
{
    char arr[1] = {0};
    int idx = 0;
    int a = arr[idx++];  // no error here
    idx -= 1;
    return arr[++idx];  // error here
}
