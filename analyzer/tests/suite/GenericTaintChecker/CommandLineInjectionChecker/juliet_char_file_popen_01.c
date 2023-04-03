#include <wchar.h>

#ifdef _WIN32
#define FULL_COMMAND "dir "
#else
#include <unistd.h>
#define FULL_COMMAND "ls "
#endif

#ifdef _WIN32
#define FILENAME "C:\\temp\\file.txt"
#else
#define FILENAME "/tmp/file.txt"
#endif

/* define POPEN as _popen on Windows and popen otherwise */
#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else /* NOT _WIN32 */
#define POPEN popen
#define PCLOSE pclose
#endif

void CWE78_OS_Command_Injection__char_file_popen_01_bad()
{
    char* data;
    char data_buf[100] = FULL_COMMAND;
    data = data_buf;
    {
        /* Read input from a file */
        size_t dataLen = strlen(data);
        FILE* pFile;
        /* if there is room in data, attempt to read the input from a file */
        if (100 - dataLen > 1) {
            pFile = fopen(FILENAME, "r");
            if (pFile != NULL) {
                /* POTENTIAL FLAW: Read data from a file */
                if (fgets(data + dataLen, (int)(100 - dataLen), pFile) == NULL) {
                    printLine("fgets() failed");
                    /* Restore NUL terminator if fgets fails */
                    data[dataLen] = '\0';
                }
                fclose(pFile);
            }
        }
    }
    {
        FILE* pipe;
        /* POTENTIAL FLAW: Execute command in data possibly leading to command injection */
        pipe = POPEN(data, "w");
        if (pipe != NULL) {
            PCLOSE(pipe);
        }
    }
}
