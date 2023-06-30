#include <cstdlib>

int EXTER_ATTACK main(int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }

    int x = std::atoi(argv[1]);
    if (x > 10)
        x = 10;
    for (int i = x - 1; i >= 0; i--) {  // No loop here
    }
}
