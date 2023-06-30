#include "std_testcase.h"

/* The two function below always return the same value, so a tool
   should be able to identify that calls to the functions will always
   return a fixed value. */
static int staticReturnsTrue()
{
    return 1;
}

static int staticReturnsFalse()
{
    return 0;
}

void CWE476_NULL_Pointer_Dereference__null_check_after_deref_08_bad()
{
    if(staticReturnsTrue())
    {
        {
            int *intPointer = NULL;
            // intPointer = (int *)malloc(sizeof(int));
            // *intPointer = 5;
            printIntLine(*intPointer);
            /* FLAW: Check for NULL after dereferencing the pointer. This NULL check is unnecessary. */
            if (intPointer != NULL)
            {
                *intPointer = 10;
            }
            printIntLine(*intPointer);
        }
    }
}

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    CWE476_NULL_Pointer_Dereference__null_check_after_deref_08_bad();
    printLine("Finished bad()");
    return 0;
}
