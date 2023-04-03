
int main()
{
    int arr[10] = {0};
    int sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += arr[2 * i];  // out of bounds
    }
    return sum;
}
