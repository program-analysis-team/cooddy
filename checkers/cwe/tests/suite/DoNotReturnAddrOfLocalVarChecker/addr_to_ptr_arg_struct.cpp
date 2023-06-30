struct B{
    int* val;
};

void foo(B* val)
{
    int x;
    val->val = &x;
}
