#include <cstdlib>
#include <iostream>

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK main(int argc, char** argv)
{
    int arr[10] = {};
    if (argc < 2) {
        return -1;
    }
    *(arr + std::atoi(argv[1]));
}
