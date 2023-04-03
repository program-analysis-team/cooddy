#ifndef UNREFERENCE_PARAM
#define UNREFERENCE_PARAM(x) ((void)(x))
#endif

int main()
{
    int Errno;
    UNREFERENCE_PARAM(Errno);
    return 0;
}
