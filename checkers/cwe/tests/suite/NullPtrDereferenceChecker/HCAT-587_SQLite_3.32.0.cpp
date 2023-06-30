int Foo(char **pointerToIter)
{
    char *theSameAsIter = *pointerToIter;  // theSameAsIter is the same as variable iter in the 'main' method
    int nEntry = 0;

    while (*theSameAsIter) {  //  theSameAsIter is nullptr, so there is null pointer dereference here
        nEntry++;
    }

    return nEntry;
}

int main()
{
    char *iter = 0;

    while (1) {
        int result = Foo(&iter);
    }
}
