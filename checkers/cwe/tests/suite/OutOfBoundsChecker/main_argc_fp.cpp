
int arr[100];

int main(int argc, char* argv[])
{
    if (argc >= 100)
        return -1;
    arr[argc] = 1;
    return 0;
}
