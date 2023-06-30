#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

int execl(const char *pathname, const char *arg, ...);

void EXTER_ATTACK f(const char* arg) {
	execl("app.exe", "-a", "-b", arg, "-d");
}
