#include <cstdlib>

int main(int argc, char** argv)
{
    if (argc < 2) {
        return -1;
    }
    std::malloc(std::atoi(argv[1]));
}
