int foo(int* value = nullptr)
{
    int b = *value;
    return b;
}

int main()
{
    return foo();
}
