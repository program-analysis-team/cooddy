#include <cstdlib>

int EXTER_ATTACK main(int argc, char** argv)
{
    if (argc < 2 || argv == nullptr || argv[1] == nullptr) {
        return -1;
    }

    int y;
    int x = std::atoi(argv[1]);
    y = x + 1;
    int i = 0;
    do {
        i++;
    } while (i < y);  // Loop here
}
