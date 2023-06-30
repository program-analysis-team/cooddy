struct B {
    int x;
    int y;
};

void init(void* ptr){
    struct B *b = (struct B*) ptr;
    b->x = 0;
    b->y = 0;
}
int intarr[] = {1, 1};

int main(int argc, char* argv[]){
    init((void*) intarr);
    return 0;
}
