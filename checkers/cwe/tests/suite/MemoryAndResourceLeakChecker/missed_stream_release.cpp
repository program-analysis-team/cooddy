#include <stdio.h>

int checkChecksum(char* buf);
void decodeBlock(char* buf);
int BUF_SZ = 1234;
int decodeFile(char* fName)
{
    char buf[BUF_SZ];
    FILE* f = fopen(fName, "r");
    if (!f) {
        printf("cannot open %s\n", fName);
        return -1;
    } else {
        while (fgets(buf, BUF_SZ, f)) {
            if (!checkChecksum(buf)) {
                return -1;
            } else {
                decodeBlock(buf);
            }
        }
    }
    fclose(f);
    return 0;
}
