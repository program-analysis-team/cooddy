void uninit_pointer_001()
{
    int a = 5;
    int *p;
    int ret;
    ret = *p; /*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}
