int* GetSome(int)
{
    return nullptr;
}

#define GET_FIELD(ref) (GetSome(0))

void foo()
{
    int a = *GET_FIELD(1);
}
