#include "iostream"
char* GetPwd();
void log(char* s) {
    printf("Password: %s", s);
}
char* foo(char* v)
{
    char* pwd;
    pwd = GetPwd();
    log(pwd);
    return pwd;
}
