#ifndef JSMIN_H
#define JSMIN_H

#include <cstddef>
#include <cstdio>

class Jsmin {
public:
    Jsmin();
    ~Jsmin();
    char* minify(const char*);

private:
    int theA;
    int theB;
    int theLookahead;
    size_t index_in;
    size_t index_out;
    const char* input_buf;
    char* output_buf;
    size_t m_size;

    int isAlphanum(int c);
    int get();
    int peek();
    int next();
    void action(int d);
    int putc(int c, FILE* stream);
};
#endif