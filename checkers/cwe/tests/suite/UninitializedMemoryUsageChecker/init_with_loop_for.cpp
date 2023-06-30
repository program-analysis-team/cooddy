int test(int var_1)
{
    return var_1 + 1;
}

int main()
{
    int ret;
    int retryCount = 3;

    for (int iter = 0; iter < retryCount; iter++) {
        ret = test(iter);
    }
    return ret;
}
