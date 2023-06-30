struct A {
    int x;
    int y;
};

struct B {
    int x;
    int y;
    int z;
};

void init(void* ptr){
    struct B *b = (struct B*) ptr;
    b->x = 0;
    b->y = 0;
    b->z = 0; // Sink
}

int main(int argc, char* argv[]){
    char cahrarr[12];
    init((void*) cahrarr);
    return 0;
}
