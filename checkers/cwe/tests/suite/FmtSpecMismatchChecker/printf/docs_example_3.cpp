#include <cstdio>
#include <string>
using namespace std;

struct Satellite {
    int x_pos;
    int y_pos;
    int z_pos;

    int sendData(FILE* earth)
    {
        // Not enough arguments for specifiers in format string.
        // Garbage data will be read from the stack, risking program crashes.
        fprintf(earth, "%d %d %d", x_pos, y_pos /* z_pos */);
    }
};
