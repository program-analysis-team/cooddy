void uninit_pointer_010_func_001 (void * vptr)
{
    /* cast void pointer to a pointer of the appropriate type */
    char * * cptr = (char * *)vptr;
    char * buf ;
    buf = (*cptr);/*Tool should detect this line as error*/ /*ERROR:Uninitialized pointer*/
}
void uninit_pointer_010 ()
{
    void *buf1;
    uninit_pointer_010_func_001(&buf1);
}
