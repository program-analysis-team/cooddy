typedef struct {
    int* result;
    int a;
    int b;
} A;

void getKvmRecyStats(A* param)
{
    A* localParam;
    int ret = 0;
    localParam = param;
    *localParam->result = ret;
    return;
}

int main()
{
    int ret;
    A a;
    a.result = &ret;
    getKvmRecyStats(&a);
    return ret;  // no error here
}
