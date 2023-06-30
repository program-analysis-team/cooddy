
typedef struct READ4resok {
    bool eof = false;
    struct {
        int offset = 0;
        int nread = 0;
    } data;
} READ4resok;

int main()
{
    READ4resok a;
    bool f = a.eof;
    return a.data.offset;
}
