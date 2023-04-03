

int main()
{
    char arr[10] = {0};
    int idx = 0;
    idx++;
    return arr[sizeof(arr) - idx];  // no error here
}
