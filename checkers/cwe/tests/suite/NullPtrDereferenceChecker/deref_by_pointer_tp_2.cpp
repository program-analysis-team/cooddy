void Foo(char **pIter)
{
    char *pEnd = *pIter;
    *pEnd = 0;
}

int main()
{
    char *pIter = 0;

    Foo(&pIter);

    return 0;
}
