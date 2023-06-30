#include <stdio.h>
struct fp_struct {
    FILE *fp;
};

struct fp_struct g_fp;

void foo() {
    g_fp.fp = fopen("file.txt", "a+");
    if (NULL == g_fp.fp) {
        fprintf(stderr, "%s", "error");
        return;
    }
    (void)fwrite("TEST", 1024, 1, g_fp.fp);
    fclose(g_fp.fp); // no report here
    g_fp.fp = NULL;
}
