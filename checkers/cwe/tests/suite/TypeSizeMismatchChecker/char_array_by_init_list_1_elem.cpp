const int SIZE = 12;

struct B {
    int x;
    int y;
    int z;
};


int main(int argc, char* argv[]){
    char maxKey[SIZE] = {0};
    B* b = (B*)maxKey;
    return b->z;
}
