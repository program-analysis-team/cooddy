int getValue(int var)
{
    return var;
}

int main()
{
    char **ppChar = 0;
    if (getValue(0)) {
        return 0;
    }
    ppChar[0] = 0;  // problem is here
}
