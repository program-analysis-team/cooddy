
int buff[10] = {0};

int some_func(const char*);

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK main(int argc, char* argv[])
{
    int num = some_func(argv[5]);
    for (int i = 0; i < num; ++i) {
        buff[i] = 0;
    }
    return 0;
}
