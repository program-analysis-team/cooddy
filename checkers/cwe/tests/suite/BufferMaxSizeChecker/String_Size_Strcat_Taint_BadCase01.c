#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KWSYS_SHARED_FORWARD_MAXPATH 100
#define KWSYS_SHARED_FORWARD_LDPATH "LDPATH"

static int TestCaseBad01(int argc, char** argvIn)
{
    char selfPath[KWSYS_SHARED_FORWARD_MAXPATH] = {0};
    const char* oldLdpath = getenv(KWSYS_SHARED_FORWARD_LDPATH);
    if (oldLdpath == NULL) {
        return -1;
    }
    strcat(selfPath, oldLdpath);
    return 0;
}

int main(int argc, char* argv[])
{
    TestCaseBad01(argc, argv);
}
