#include <cstdlib>

int EXTER_ATTACK main(int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }

    int x = std::atoi(argv[1]);
    if (x > 10)
        x = 10;
    else
        x = 5;
    for (size_t i = 0; i < x; i++) { // No loop here
    }
}
