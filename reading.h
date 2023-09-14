#ifndef READING_H
#define READING_H

#include <stdio.h>

struct textArray 
{
    size_t size;
    size_t nLines;

    char *buffer;
    char **ptr;
    size_t *indexArray;
};

void destroyTextArray(textArray *text);

int fileSize(const char *name);

int readTextFromFile(const char *fileName, textArray *text);

char *readToBuffer(FILE *f, size_t size);

void printStr(char *str, size_t size);

int splitBufToStrings(textArray *text);

size_t countLines(const char *buffer);

void printText(char **text, size_t nLines);

void myPuts(const char *str);

int createIndexArr(textArray *text);

char getElement(const textArray *text, size_t i, size_t j);

void indexTesting(textArray *text);

#endif // READING_H