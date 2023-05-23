struct FILE {};
int printf(const char* const _Format, ...);
int fprintf(FILE* const _Stream, const char* const _Format, ...);

int printf_s(const char* const _Format, ...);
int fprintf_s(FILE* const _Stream, const char* const _Format, ...);

int wprintf(const wchar_t* const _Format, ...);
int fwprintf(FILE* const _Stream, const wchar_t* const _Format, ...);

int wprintf_s(const wchar_t* const _Format, ...);
int fwprintf_s(FILE* const _Stream, const wchar_t* const _Format, ...);
int fwrite(const void *buf, int cnt, int cnt2, FILE *fp);

FILE* file;
SENSI_INFO char* pwd = "12345";
SENSI_INFO wchar_t* wpwd = L"12345";
void printf_test()
{
    printf("Password: %s", pwd);
}
void printf_test2()
{
    printf(pwd);
}
void fprintf_test()
{
    fprintf(file, "Password: %s", pwd);
}
void fprintf_test2()
{
    fprintf(file, pwd);
}
void printf_s_test()
{
    printf_s("Password: %i %s", 1, pwd);
}
void printf_s_test2()
{
    printf_s(pwd);
}
void fprintf_s_test()
{
    fprintf_s(file, "Password: %i %s", 1, pwd);
}
void fprintf_s_test2()
{
    fprintf_s(file, pwd);
}
void wprintf_test()
{
    wprintf(L"Password: %s", wpwd);
}
void wprintf_test2()
{
    wprintf(wpwd);
}
void wfprintf_test()
{
    fwprintf(file, L"Password: %s", wpwd);
}
void wfprintf_test2()
{
    fwprintf(file, wpwd);
}
void wprintf_s_test()
{
    wprintf_s(L"Password: %s", wpwd);
}
void wprintf_s_test2()
{
    wprintf_s(wpwd);
}
void fwprintf_s_test()
{
    fwprintf_s(file, L"Password: %s", wpwd);
}
void fwprintf_s_test2()
{
    fwprintf_s(file, wpwd);
}
void fwrite_test()
{
    fwrite(pwd, 1, 1, file);
}
