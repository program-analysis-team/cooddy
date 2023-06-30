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
    struct A *b = (struct A*) ptr;
    b->x = 0;
    b->y = 0;
}

int main(int argc, char* argv[]){
    int intarr[] = {1, 1};
    init((void*) intarr);
    return 0;
}
