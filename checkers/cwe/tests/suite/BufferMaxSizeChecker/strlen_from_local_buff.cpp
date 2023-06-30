extern "C" {
int strncat_s(char* dstStr, int dstMax, char* srcStr, int srcSize);
int strlen(const char* str);
}

void get_str(char* buff, int size);

int foo()
{
    char buff[20], str1[10], str2[20];
    get_str(str1, sizeof(str1));
    get_str(str2, sizeof(str2));

    strncat_s(buff, sizeof(buff), str1, strlen(str1));      // no error here
    strncat_s(buff, sizeof(buff), str2, strlen(str2) + 2);  // error here
}
