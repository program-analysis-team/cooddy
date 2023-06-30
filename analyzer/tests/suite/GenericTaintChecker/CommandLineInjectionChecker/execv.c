#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

// #include <unistd.h>

int execv(const char *pathname, char *const argv[]);

void EXTER_ATTACK f(char* arg) {
    char* argv[] = {"app.exe", "-a", "-b", arg};

    execv("app.exe", argv);
}
