#ifndef LILSTRING_H
#define LILSTRING_H

typedef struct sString {
    char symbol;
    struct sString *next;
} *String;

void addToStr(String str, char c);
void eraseStr(String str);
void printString(String);
int strLength(String str);
char charAtStr(String str, int index);
char * strToCharArray(String str);

#endif
