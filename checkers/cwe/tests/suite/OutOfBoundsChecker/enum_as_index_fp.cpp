enum CODE_E {
    CODE_OK,
    CODE_SOME_ERROR,
    CODE_ANOTHER_ERROR,
    CODE_BAD_CODE,
    CODE_GOOD,
    CODE_X_CODE,
    CODE_LAST
};

int arr[8];

int foo(CODE_E code)
{
    arr[code]++;
    return 0;
}
