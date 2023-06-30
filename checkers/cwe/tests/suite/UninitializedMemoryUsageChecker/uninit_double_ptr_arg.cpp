void uninit_pointer_006_func_001(int **pp)
{
    **pp = 1; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}

void uninit_pointer_006()
{
    int a;
    int *p;
    uninit_pointer_006_func_001(&p);
}
