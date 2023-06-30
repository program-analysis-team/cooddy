void foo(unsigned short *ptr){};

int main() {
    unsigned int buffer = 0;
    foo(&buffer);
    return 0;
}
