struct B{
    int* val;
};

void foo(B* val)
{
    int x;
    val->val = &x;
    if (val->val != 0) {
        val->val = (int*)0x1234;
    }
}
