#include <stdlib.h>
#include <stdio.h>

static char *one_input_line(FILE *in, char *zPrior, int isContinuation){
    char *zResult;
    #if SHELL_USE_LOCAL_GETLINE
        printf("%s", zPrompt);
        fflush(stdout);
        zResult = local_getline(zPrior, stdin);
    #else
        free(zPrior);
        zResult = (char *) malloc(100);
    #endif
    return zResult;
}


static int process_input(){
    char *zLine = 0;          /* A single input line */
    int nSql = 0;             /* Bytes of zSql[] used */
    zLine = one_input_line(nullptr, zLine, nSql>0);
    return zLine[0];
}
