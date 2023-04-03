
int idx[][3] = {{2, 3, 4}, {1, 6, 3}, {8, 9, 10}};
int arr[5];

void foo(int p)
{
    arr[idx[0][p]] = 0;  // no error here
    arr[idx[1][p]] = 0;  // error here
}
