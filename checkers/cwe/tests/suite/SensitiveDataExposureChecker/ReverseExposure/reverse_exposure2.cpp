#include "iostream"
char* GetPwd();
void log(char* s) {
    printf("Password: %s", s);
}
void myLogin(SENSI_INFO const char* pwd){
    pwd; // param var decl should be referenced
}
char* foo(char* pwd)
{
    log(pwd);
    myLogin(pwd);
    return pwd;
}
