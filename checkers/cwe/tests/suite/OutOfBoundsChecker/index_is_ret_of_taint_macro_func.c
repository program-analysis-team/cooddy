#ifndef EXTER_ATTACK
#define EXTER_ATTACK
#endif

int EXTER_ATTACK taint();

int EXTER_ATTACK main(int argc, char **argv) {
    int arr[4] = {0, 1, 2, 3};
    int index = taint();
    return arr[index];
}
