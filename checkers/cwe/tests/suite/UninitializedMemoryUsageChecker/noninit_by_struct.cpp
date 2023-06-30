struct A {
    int *result;
    int a;
};

void foo(A& param){
    param.a = 123;
}

int goo()
{
    int ret2;
    A params;
    params.result = &ret2;
    foo(params);
    return ret2; //error here
}
