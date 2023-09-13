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
    //indexTesting(ptr, nLines, size);
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

    text->nLines = 0;
    text->ptr = (char **)calloc(text->nLines, sizeof(char *));

    if (text->ptr == NULL)
    {
        return 1;
    }

    *text->ptr++ = text->buffer;
    text->nLines++;

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
            text->nLines++;
        }
    }
    text->ptr -= text->nLines;
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

    free(text->buffer);
    text->buffer = NULL;

    free(text->ptr);
    text->ptr    = NULL;

    text->size   = -1;
    text->nLines = -1;
}

size_t *createIndexArr(char **text, size_t nLines)
{
    assert(text);

    size_t *indexArray = (size_t *)calloc(nLines, sizeof(size_t));
    if (indexArray == NULL)
    {
        printf("coouldn't allocate index array\n");
        return NULL;
    }

    indexArray[0] = 0;
    for (size_t i = 1; i < nLines; i++)
    {
        printf("text[i] = %p, text[i - 1] = %p\n", text[i], text[i - 1]);
        indexArray[i] = indexArray[i - 1] + (text[i] - text[i - 1]);
    }

    return indexArray;
}

char getElement(char *buffer, size_t *indexArray, size_t bufSize, size_t nLines, size_t i, size_t j)
{
    assert(buffer);
    assert(indexArray);
    
    assert(i < nLines);
    size_t index = indexArray[i] + j;
    assert(index < bufSize);
    if (i < nLines - 1)
    {
        assert(index < indexArray[i + 1]);
    }

    return buffer[index];
}

void indexTesting(char **text, size_t nLines, size_t size)
{
    // index testing
    size_t *indexArray = createIndexArr(text, nLines);
    for (size_t i = 0; i < nLines; i++)
    {
        printf("%lld ", indexArray[i]);
        myPuts(text[i]);
    }

    putchar(getElement(*text, indexArray, size, nLines, 1, 2));
    putchar(getElement(*text, indexArray, size, nLines, 1, 3));
    putchar(getElement(*text, indexArray, size, nLines, 1, 4));
    putchar(getElement(*text, indexArray, size, nLines, 2, 0));

    free(indexArray);
}
