
int main()
{
    int arr[10] = {0};
    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += arr[i];  // no error here
    }
    return sum;
}
