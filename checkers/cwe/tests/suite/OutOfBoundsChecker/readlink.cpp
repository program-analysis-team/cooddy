
int readlink(const char* path, char* buf, unsigned int bufsiz);

void f1(const char* path)
{
    char buf[100];
    int ret = readlink(path, buf, 99);
    if(ret<0)
        return;
    buf[ret] = 0; //good
}
void f2(const char* path)
{
    char buf[100];
    int ret = readlink(path, buf, 100);
    if(ret<0)
        return;
    buf[ret] = 0; //bad
}
