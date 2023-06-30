int main()
{
    char* pointer = nullptr;
    int x = 0;
    if (x == 1) {
        char c = 0;
        //	Null pointer derefence is found at the line '*pointer = 0;' if next line is commented.
        //	Mentioned Null pointer derefence isn't found if next line isn't commented.
        const decltype(*pointer) y = c;
    }

    *pointer = 0;  //	Actually, there is null pointer dereference
}
