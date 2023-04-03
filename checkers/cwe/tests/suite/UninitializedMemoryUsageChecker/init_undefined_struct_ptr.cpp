struct A {
    int *result;
    int a;
};

void getKvmRecyStats(A *param);

int main()
{
    int ret2;
    A params;
    params.result = &ret2;
    getKvmRecyStats(&params);

    return ret2;  // no error here
}
