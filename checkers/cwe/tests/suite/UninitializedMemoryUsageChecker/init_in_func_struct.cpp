struct A {
    int *result;
    int a;
};

void getKvmRecyStats(A &param)
{
    *param.result = 0;
}

int main()
{
    int ret2;
    A params;
    params.result = &ret2;
    getKvmRecyStats(params);

    return ret2;  // no error here
}
