int taint_return();
void taint_out_param(int* x);

int taint_return_legacy();
void taint_out_param_legacy(int* x);

int taint_return_2()
{
    return 0;
}

int taint_args(int arg1_taint, int arg2_no_taint, int arg3_taint)
{
    int arr[10] = {};

    arr[arg1_taint];
    arr[arg2_no_taint];
    arr[arg3_taint];
}

int foo()
{
    int arr[10] = {};

    arr[taint_return()];
    arr[taint_return_2()];

    int x1 = 0;
    taint_out_param(&x1);
    arr[x1];

    arr[taint_return_legacy()];

    int x2 = 0;
    taint_out_param_legacy(&x2);
    arr[x2];
}
