#include <cstdlib>

int EXTER_ATTACK main(int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }

    int x = std::atoi(argv[1]);
    int i = 0;
    while (2 * i < 2 * x + 3) {
        ++i;
    }
}
