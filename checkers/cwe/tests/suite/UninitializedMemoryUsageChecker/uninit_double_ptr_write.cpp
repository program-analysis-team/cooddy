void uninit_pointer_003 ()
{
    int **pp;
    int *p;
    int a = 0;
    int ret;
    pp = &p;
    ret = **pp; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}
