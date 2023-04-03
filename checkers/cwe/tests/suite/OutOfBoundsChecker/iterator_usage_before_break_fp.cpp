
int retVal(int val)
{
    return val;
}

int main()
{
    int arr[5] = {};
    int j = 0;

    for (int i = 0; i < 4; i++) {
        j = retVal(i);
        if (0 == j) {
            continue;
        }

        arr[i];  // no error here
        break;
    }
}
