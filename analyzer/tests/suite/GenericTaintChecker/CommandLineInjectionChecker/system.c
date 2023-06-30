#include <stdlib.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

int EXTER_ATTACK main(int argC, char** argV)
{
    if (argC > 1) {
        system(argV[1]);
    }
}
