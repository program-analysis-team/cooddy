#include "string"
void log(const char* s) {
    printf("Password: %s", s);
}
const char* GetPwd() {
    SENSI_INFO const char* pwd = "!!!";
    return pwd;
}
void foo()
{
    const char* pwd;
    pwd = GetPwd();
    log(pwd);
}