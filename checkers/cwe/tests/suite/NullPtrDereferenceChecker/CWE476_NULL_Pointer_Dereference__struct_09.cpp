#include "std_testcase.h"

#include <wchar.h>

void CWE476_NULL_Pointer_Dereference__struct_09_bad()
{
    twoIntsStruct * data;
    if(GLOBAL_CONST_TRUE)
    {
        /* POTENTIAL FLAW: Set data to NULL */
        data = NULL;
    }
    if(GLOBAL_CONST_TRUE)
    {
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        printIntLine(data->intOne);
    }
}

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__struct_09_bad();
    printLine("Finished bad()");
    return 0;
}
