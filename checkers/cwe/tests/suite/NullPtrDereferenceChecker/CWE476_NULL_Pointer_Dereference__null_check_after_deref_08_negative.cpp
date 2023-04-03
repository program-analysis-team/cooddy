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

/* good1() uses if(staticReturnsFalse()) instead of if(staticReturnsTrue()) */
static void good1()
{
    if (staticReturnsFalse()) {
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
    } else {
        {
            int *intPointer = NULL;
            intPointer = (int *)malloc(sizeof(int));
            if (intPointer != nullptr) {
                *intPointer = 5;
                printIntLine(*intPointer);
                /* FIX: Don't check for NULL since we wouldn't reach this line if the pointer was NULL */
                *intPointer = 10;
                printIntLine(*intPointer);
            }
        }
    }
}

/* good2() reverses the bodies in the if statement */
static void good2()
{
    if (staticReturnsTrue()) {
        {
            int *intPointer = NULL;
            intPointer = (int *)malloc(sizeof(int));
            if (intPointer != nullptr) {
                *intPointer = 5;
                printIntLine(*intPointer);
                /* FIX: Don't check for NULL since we wouldn't reach this line if the pointer was NULL */
                *intPointer = 10;
                printIntLine(*intPointer);
            }
        }
    }
}

void CWE476_NULL_Pointer_Dereference__null_check_after_deref_08_good()
{
    good1();
    good2();
}

int main(int argc, char *argv[])
{
    /* seed randomness */
    srand((unsigned)time(NULL));
    printLine("Calling good()...");
    CWE476_NULL_Pointer_Dereference__null_check_after_deref_08_good();
    printLine("Finished good()");
    return 0;
}
