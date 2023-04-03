

int some_func(char*);

int main(int argc, char* argv[])
{
    int buff[10] = {0};
    int idx = some_func(argv[0]);
    return buff[idx];
}
