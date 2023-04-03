
// grouping by last call which return source point

int* get_ptr(int v)
{
    static int ret = 0;
    if (v == 1) {
        return 0;
    }
    return &ret;
}

int test1(int v)
{
    int* p1 = get_ptr(v);
    int* p2 = get_ptr(v);
    return *p1 + *p2;
}

// grouping by last call with source point as argument

void init(int v, int** p)
{
    if (v == 1) {
        return;
    }
    *p = 0;
}

int check(int v)
{
    return v == 0 ? 0 : 1;
}

int test2(int v)
{
    int *p1, *p2;
    init(v, &p1);
    init(v, &p2);
    check(v);  // dummy call
    check(v);  // dummy call
    return *p1 + *p2;
}

// grouping by last call before sink

int undef_call();

int test3(int v)
{
    int p1, p2;
    if (undef_call() >= 0) {
        p1 = 10;
    }
    if (undef_call() < 0) {
        p2 = 15;
    }
    return p1 + p2;
}

// grouping by recordType in sizeof

int memcpy_s(const void* dstBuff, int dstMax, const void* srcBuff, int srcSize);

struct MyStruct {
    int a;
};

int test4(const char* v)
{
    char dstBuff[1];
    struct MyStruct a;
    memcpy_s(dstBuff, sizeof(dstBuff), &a, sizeof(a));

    struct MyStruct b;
    memcpy_s(dstBuff, sizeof(dstBuff), &b, sizeof(struct MyStruct));
}
