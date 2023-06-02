#include "iostream"
struct SENSI_INFO S {
    char* pwd;
};
void log(char* s) {
    printf("Password: %s", s);
}
void foo(S s)
{
    log(s.pwd);
}