#include <processthreadsapi.h>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

void EXTER_ATTACK f(const char* newPath) {
    const char* environment[] = {newPath, NULL};

    CreateProcessA("app.exe", "-x", NULL, NULL, 0, 0, environment, ".", NULL, NULL);
}
