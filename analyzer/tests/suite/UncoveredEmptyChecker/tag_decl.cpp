struct rectangle{
    int a, b;
    rectangle(int x, int y):a(x), b(y){}
};

int my_fun(int b) {
    rectangle res(3,4);
    return res.a;
}
