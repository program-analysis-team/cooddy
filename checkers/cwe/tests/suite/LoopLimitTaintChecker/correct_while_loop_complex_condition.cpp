#include <cstdlib>

int EXTER_ATTACK main(int argc, char** argv)
{
    if (argc > 10)
        argc = 10;
    int i = 0;
    while (2 * i < 2 * argc + 3) {
        ++i;
    }
}
