int someCondition();

int baz(int *x)
{
    while(1) {
        if (someCondition())
            break;
        auto ret = someCondition();
        if (ret!=0) {
            return ret;
        }
    }
    if (x){
        *x=123;
    }
    return 0;
}

int bar()
{
    int x;
    auto ret = baz(&x);
    if (ret!=0)
        return 1;
    return x;
}
