/* header file to define functions in io.c.  Not named io.h
   because that name is already taken by a system header on
   Windows */

#ifndef _STD_TESTCASE_IO_H
#define _STD_TESTCASE_IO_H

#include "std_testcase.h" /* needed for the twoint struct */

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h> // for PRId64
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
#include "std_testcase.h"

#ifndef _WIN32
#include <wchar.h>
#endif

void printLine (const char * line)
{
    if(line != NULL)
    {
        printf("%s\n", line);
    }
}

void printWLine (const wchar_t * line)
{
    if(line != NULL)
    {
        wprintf(L"%ls\n", line);
    }
}

void printIntLine (int intNumber)
{
    printf("%d\n", intNumber);
}

void printShortLine (short shortNumber)
{
    printf("%hd\n", shortNumber);
}

void printFloatLine (float floatNumber)
{
    printf("%f\n", floatNumber);
}

void printLongLine (long longNumber)
{
    printf("%ld\n", longNumber);
}

void printLongLongLine (int64_t longLongIntNumber)
{
    printf("%" PRId64 "\n", longLongIntNumber);
}

void printSizeTLine (size_t sizeTNumber)
{
    printf("%zu\n", sizeTNumber);
}

void printHexCharLine (char charHex)
{
    printf("%02x\n", charHex);
}

void printWcharLine(wchar_t wideChar)
{
    /* ISO standard dictates wchar_t can be ref'd only with %ls, so we must make a
     * string to print a wchar */
    wchar_t s[2];
    s[0] = wideChar;
    s[1] = L'\0';
    printf("%ls\n", s);
}

void printUnsignedLine(unsigned unsignedNumber)
{
    printf("%u\n", unsignedNumber);
}

void printHexUnsignedCharLine(unsigned char unsignedCharacter)
{
    printf("%02x\n", unsignedCharacter);
}

void printDoubleLine(double doubleNumber)
{
    printf("%g\n", doubleNumber);
}

void printStructLine (const twoIntsStruct * structTwoIntsStruct)
{
    printf("%d -- %d\n", structTwoIntsStruct->intOne, structTwoIntsStruct->intTwo);
}

void printBytesLine(const unsigned char * bytes, size_t numBytes)
{
    size_t i;
    for (i = 0; i < numBytes; ++i)
    {
        printf("%02x", bytes[i]);
    }
    puts("");	/* output newline */
}

/* Decode a string of hex characters into the bytes they represent.  The second
 * parameter specifies the length of the output buffer.  The number of bytes
 * actually written to the output buffer is returned. */
size_t decodeHexChars(unsigned char * bytes, size_t numBytes, const char * hex)
{
    size_t numWritten = 0;

    /* We can't sscanf directly into the byte array since %02x expects a pointer to int,
     * not a pointer to unsigned char.  Also, since we expect an unbroken string of hex
     * characters, we check for that before calling sscanf; otherwise we would get a
     * framing error if there's whitespace in the input string. */
    while (numWritten < numBytes && isxdigit(hex[2 * numWritten]) && isxdigit(hex[2 * numWritten + 1]))
    {
        int byte;
        sscanf(&hex[2 * numWritten], "%02x", &byte);
        bytes[numWritten] = (unsigned char) byte;
        ++numWritten;
    }

    return numWritten;
}

/* Decode a string of hex characters into the bytes they represent.  The second
 * parameter specifies the length of the output buffer.  The number of bytes
 * actually written to the output buffer is returned. */
//size_t decodeHexWChars(unsigned char * bytes, size_t numBytes, const wchar_t * hex)
//{
//    size_t numWritten = 0;
//
//    /* We can't swscanf directly into the byte array since %02x expects a pointer to int,
//     * not a pointer to unsigned char.  Also, since we expect an unbroken string of hex
//     * characters, we check for that before calling swscanf; otherwise we would get a
//     * framing error if there's whitespace in the input string. */
//    while (numWritten < numBytes && iswxdigit(hex[2 * numWritten]) && iswxdigit(hex[2 * numWritten + 1]))
//    {
//        int byte;
//        swscanf(&hex[2 * numWritten], L"%02x", &byte);
//        bytes[numWritten] = (unsigned char) byte;
//        ++numWritten;
//    }
//
//    return numWritten;
//}

/* The two functions always return 1 or 0, so a tool should be able to
   identify that uses of these functions will always return these values */
int globalReturnsTrue()
{
    return 1;
}

int globalReturnsFalse()
{
    return 0;
}

int globalReturnsTrueOrFalse()
{
    return (rand() % 2);
}

/* Define some global variables that will get argc and argv */
extern int globalArgc;
extern char** globalArgv;

#ifdef __cplusplus
}
#endif

#endif
