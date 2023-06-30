#include "std_testcase.h"

#include <wchar.h>

void CWE476_NULL_Pointer_Dereference__wchar_t_10_bad()
{
    wchar_t * data;
    if(globalTrue)
    {
        /* POTENTIAL FLAW: Set data to NULL */
        data = NULL;
    }
    if(globalTrue)
    {
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        /* printWLine() checks for NULL, so we cannot use it here */
        printWcharLine(data[0]);
    }
}

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__wchar_t_10_bad();
    printLine("Finished bad()");
    return 0;
}
