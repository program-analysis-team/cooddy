#include "iostream"
char* GetPwd();
void log(char* s) {
    printf("Password: %s", s);
}
void login(const char* pwd);
char* foo(char* pwd)
{
    log(pwd);
    login(pwd);
    return pwd;
}