int foo(int* value)
{
    int b = *value;
    return b;
}

int main()
{
    return foo(nullptr);
}
