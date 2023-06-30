#include <processthreadsapi.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK f(const char* untrusted) {
    CreateProcessA(untrusted, "-x", NULL, NULL, FALSE, NULL, NULL, ".", NULL, NULL);
}
