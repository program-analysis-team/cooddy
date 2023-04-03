// Source: input parameter of an entry point
// Propagation: structure member
// Path sensitivity: condition in loop
// Sink: pointer arithmetic

struct Data {
    int x;
    int y;
    int z;
};

#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif // EXTER_ATTACK

int EXTER_ATTACK main(int argc, char** argv)
{
    int arr[10] = {};

    struct Data d = {};
    d.z = argc;

    for (int i = 0; i < 1; ++i) {
        *(arr + d.z) = 0;
    }
}
