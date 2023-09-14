#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include "reading.h"

int main()
{
    const char *fileName = "file.txt";

    textArray text = {};
    readTextFromFile(fileName, &text);

    printText(text.ptr, text.nLines);
    printStr(text.buffer, text.size);

    putchar('\n');
    indexTesting(&text);
    destroyTextArray(&text);
}

int readTextFromFile(const char *fileName, textArray *text)
{
    assert(text);
    assert(fileName);

    text->size = fileSize(fileName) + 1;
    if (!text->size)  
    { 
        perror("ERROR");
        return 1; 
    }
    printf("size = %lld\n", text->size);

    FILE *f = fopen(fileName, "r");
    if (!f)      
    { 
        perror("ERROR"); 
        return 1; 
    }

    text->buffer = readToBuffer(f, text->size);
    if (!text->buffer) 
    { 
        perror("ERROR"); 
        return 1; 
    }
    fclose(f);

    printStr(text->buffer, text->size);

    splitBufToStrings(text);
    if (!text->ptr) 
    { 
        perror("ERROR"); 
        return 1; 
    }
    text->indexArray = NULL;

    return 0;
}

int fileSize(const char *name)
{
    assert(name);

    struct stat stats = {};
    if (stat(name, &stats) == -1)
    {
        return -1;
    }

    int size = stats.st_size;
    return size;
}

char *readToBuffer(FILE *f, size_t size)
{
    assert(f);

    char *buffer = (char *)calloc(size, sizeof(char));
    if (buffer == NULL)
    {
        return NULL;
    }
    fread(buffer, sizeof(char), size, f);

    return buffer;
}

void printStr(char *str, size_t size)
{
    assert(str);

    for (size_t i = 0; i < size; i++)
    {
        printf("(%d)%c", str[i], str[i]);
    }
}

int splitBufToStrings(textArray *text)
{
    assert(text);
    assert(text->buffer);

    text->nLines = countLines(text->buffer);
    text->ptr = (char **)calloc(text->nLines, sizeof(char *));

    if (text->ptr == NULL)
    {
        return 1;
    }

    *text->ptr++ = text->buffer;
    size_t line = 1;

    for (size_t i = 0 ; *(text->buffer + i) != '\0'; i++)
    {
        printf("separating the line, current elem is (%d)\n", *(text->buffer + i));
        if (*(text->buffer + i) == '\n')
        {
            printf("*buffer is '\\n'\n");
            printf("ptr  = %p\n", text->ptr);
            printf("*ptr = %p\n", *text->ptr);
            *text->ptr = text->buffer + i + 1;
            printf("*ptr = %p\n", *text->ptr);
            text->ptr++;
            line++;
        }
    }
    text->ptr -= line;
    return 0;
}

size_t countLines(const char *buffer)
{
    assert(buffer);

    size_t count = 0;
    while (*buffer != '\0')
    {
        if (*buffer == '\n')
        {
            count++;
        }
        buffer++;
    }
    return count + 1; 
}

void printText(char **text, size_t nLines)
{
    assert(text);

    for (size_t i = 0; i < nLines; i++)
    {
        putchar('<');
        myPuts(text[i]);
        putchar('>');
        printf(", address = %p\n", text[i]);
    }
}

void myPuts(const char *str)
{
    assert(str);

    for ( ; *str != '\0' && *str != '\n'; str++)
    {
        putchar(*str);
    }
    putchar('\n');
}

void destroyTextArray(textArray *text)
{
    assert(text);

    if (text->buffer)
    {
        free(text->buffer);
        text->buffer = NULL;
    }

    if (text->ptr)
    {
        free(text->ptr);
        text->ptr = NULL;
    }

    if (text->indexArray)
    {
        free(text->indexArray);
        text->indexArray = NULL;
    }

    text->size   = -1;
    text->nLines = -1;
}

int createIndexArr(textArray *text)
{
    assert(text);

    text->indexArray = (size_t *)calloc(text->nLines, sizeof(size_t));
    if (text->indexArray == NULL)
    {
        printf("couldn't allocate index array\n");
        return 1;
    }

    text->indexArray[0] = 0;
    for (size_t i = 1; i < text->nLines; i++)
    {
        printf("text[i] = %p, text[i - 1] = %p\n", text->ptr[i], text->ptr[i - 1]);
        text->indexArray[i] = text->indexArray[i - 1] + (text->ptr[i] - text->ptr[i - 1]);
        //printf("indexArray[%lld] = %lld\n", i, text->indexArray[i]);
    }

    return 0;
}

char getElement(const textArray *text, size_t i, size_t j)
{
    assert(text->buffer);
    assert(text->indexArray);
    
    assert(i < text->nLines);
    size_t index = text->indexArray[i] + j;
    assert(index < text->size);
    if (i < text->nLines - 1)
    {
        assert(index < text->indexArray[i + 1]);
    }

    return text->buffer[index];
}

void indexTesting(textArray *text)
{
    // index testing

    createIndexArr(text);

    for (size_t i = 0; i < text->nLines; i++)
    {
        printf("%lld ", text->indexArray[i]);
        myPuts(text->ptr[i]);
    }

    putchar(getElement(text, 1, 2));
    putchar(getElement(text, 1, 3));
    putchar(getElement(text, 1, 4));
    putchar(getElement(text, 2, 0));
}
