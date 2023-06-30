#include <pthread.h>
#include <stdlib.h>
#include <iostream>

void my_open(FILE* m);
void my_close(FILE* m);

void test()
{
    FILE file;
    my_open(&file);
    my_close(&file);  // no detect here
}
void test2()
{
    FILE file;
    my_open(&file);
    fclose(&file);  // detect here
}
