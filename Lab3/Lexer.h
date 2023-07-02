#ifndef LAB3_LEXER_H
#define LAB3_LEXER_H

#include "stdbool.h"

int lexer();

bool readFile();

int analyze();

int findIndexOfStringEnd(char *string, int start, const char *quote);

int compareSubstring(const char *string, int start, const char *comparable);

int containsIn(const char *, const char *[], int);

int analyzeChar(char i);

#endif
