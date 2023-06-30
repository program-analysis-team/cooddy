#include "std_testcase.h"

#include <wchar.h>

void CWE476_NULL_Pointer_Dereference__char_02_bad()
{
    char * data;
    if(1)
    {
        /* POTENTIAL FLAW: Set data to NULL */
        data = nullptr;
    }
    if(1)
    {
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        /* printLine() checks for NULL, so we cannot use it here */
        printHexCharLine(data[0]);
    }
}

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__char_02_bad();
    printLine("Finished bad()");
    return 0;
}
