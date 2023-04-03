#define RETURN_OP_BUTT (-7)

char* g_arr[(~RETURN_OP_BUTT) + 1];

void foo()
{
    unsigned int i = 0;
    for (i = 1; i < ((~RETURN_OP_BUTT) + 1); i++) {
        g_arr[i] = 0;  // no error here
    }
}
