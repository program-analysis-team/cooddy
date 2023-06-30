void uninit_memory_access_005()
{
    int i, j, k;
    int arr[2][3][4] = {{{1, 2, 3, 4}, {12, 22, 32, 42}, {13, 23, 33, 43}},
                        {{21, 22, 23, 24}, {31, 32, 33, 34}, {41, 42, 43, 44}}};
    int arr1[2][3][4];
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 1; j++) {
            for (k = 0; k < 1; k++) {
                arr1[i][j][k] = arr[i][j][k];
            }
        }
    }
    k = arr1[1][2][3]; /*Tool should detect this line as error*/ /*ERROR:Uninitialized Memory Access*/
}
