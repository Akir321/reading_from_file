#ifndef READING_H
#define READING_H

#include <stdio.h>

struct textArray 
{
    size_t size;
    size_t nLines;

    char *buffer;
    char **ptr;
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

size_t *createIndexArr(char **text, size_t nLines);

char getElement(char *buffer, size_t *indexArray, size_t bufSize, size_t nLines, size_t i, size_t j);

void indexTesting(char **text, size_t nLines, size_t size);

#endif // READING_H