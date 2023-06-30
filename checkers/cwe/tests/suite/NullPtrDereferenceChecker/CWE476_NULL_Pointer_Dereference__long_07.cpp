#include "std_testcase.h"

#include <wchar.h>

/* The variable below is not declared "const", but is never assigned
   any other value so a tool should be able to identify that reads of
   this will always give its initialized value. */
static int staticFive = 5;

void CWE476_NULL_Pointer_Dereference__long_07_bad()
{
    long * data;
    if(staticFive==5)
    {
        /* POTENTIAL FLAW: Set data to NULL */
        data = NULL;
    }
    if(staticFive==5)
    {
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        printLongLine(*data);
    }
}

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__long_07_bad();
    printLine("Finished bad()");
    return 0;
}
