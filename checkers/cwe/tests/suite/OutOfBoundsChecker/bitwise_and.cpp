int main(int argc, char** argv)
{
    int buff[256];
    auto idx = ((4294967295 & 0xff000000) >> 24);  // idx equals 255
    buff[idx];
    return 0;
}
