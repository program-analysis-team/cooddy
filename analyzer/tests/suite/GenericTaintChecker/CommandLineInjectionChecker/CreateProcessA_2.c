#include <processthreadsapi.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK f(char* untrusted) {
    CreateProcessA("app.exe", untrusted, NULL, NULL, FALSE, NULL, NULL, ".", NULL, NULL);
}
