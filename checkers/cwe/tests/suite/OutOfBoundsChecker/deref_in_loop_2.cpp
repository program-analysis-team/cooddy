
int main()
{
    int arr[10] = {0};
    int sum = 0;
    for (int i = 10; i >= 0; i--) {
        sum += arr[10 - i];  // out of bounds
    }
    return sum;
}
