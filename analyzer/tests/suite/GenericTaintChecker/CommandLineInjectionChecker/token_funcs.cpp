#include <unistd.h>
#include <string.h>


#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif  // EXTER_ATTACK

int EXTER_ATTACK test(char* str)
{
    char* pch = strtok(str, " ,.-");
    execl(pch, pch, "1", "2", nullptr);
    return 0;
}
