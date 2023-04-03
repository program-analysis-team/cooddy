#include "std_testcase.h"

namespace CWE476_NULL_Pointer_Dereference__class_03 {

void bad() {
    TwoIntsClass *data;
    if (5 == 5) {
        /* POTENTIAL FLAW: Set data to NULL */
        data = NULL;
    }
    if (5 == 5) {
        /* POTENTIAL FLAW: Attempt to use data, which may be NULL */
        printIntLine(data->intOne);
        /* avoid memory leak - deleting a NULL pointer is a no-op in C++ so no need to check for NULL */
        delete data;
    }
}

}  // namespace

using namespace CWE476_NULL_Pointer_Dereference__class_03; /* so that we can use good and bad easily */

int main(int argc, char * argv[])
{
    /* seed randomness */
    srand( (unsigned)time(NULL) );
    printLine("Calling bad()...");
    bad();
    printLine("Finished bad()");
    return 0;
}
