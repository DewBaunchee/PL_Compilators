#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TokenStream.h"
#include "Lexer.h"
#include "Stack.h"

#define NELEMS(a) sizeof((a)) / sizeof((a)[0])

const char *keyWords[] = {"False", "True", "None", "and", "break", "continue", "def",
                          "elif", "else", "except", "finally", "for", "global", "if",
                          "import", "in", "not", "or", "return", "try", "while", "as"};

const char *operators[] = {"+", "-", "*", "/", "**", "//", "%", "<", ">", "<=", ">=",
                           "==", "!=", "=", "+=", "-=", "/=", "*=", "%=", "**=", "//="};

const char *separators[] = {"+", "-", "*", "/", "%", "<", ">", "=", "!", "%",
                            "(", ")", "[", "]", "{", "}", ",", ".", ":",
                            " ", "\t", "\n", "\"", "'"};

unsigned long size;
char *allChars;
enum type {
    tLetter, tDigit, tSeparator
};

Stack nestStack;
String temp;
enum classes tempClass;
int curIndex;
int isNesting;
int addingNest;

bool recNestCheck();

char *returnErrorLine();

enum type getType(char c) {
    if (c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return tLetter;
    if (c >= '0' && c <= '9') return tDigit;

    char c0[2];
    c0[0] = c;
    c0[1] = 0;
    if (containsIn(c0, separators, NELEMS(separators))) return tSeparator;
    return -1;
}

// ../analyzable/lab1.py
int analyze() {
    nestStack = NULL;
    nestStack = pushStack(nestStack, 0);
    isNesting = 0;
    addingNest = 0;
    tempClass = -1;
    temp = calloc(1, sizeof(struct sString));

    for (curIndex = 0; curIndex <= size; curIndex++) {
        if (allChars[curIndex] == '#') {
            while (allChars[curIndex] != '\n') curIndex++;
            isNesting = 1;
        }

        if (allChars[curIndex] == '"') {
            int quoteEnd;
            if (allChars[curIndex + 1] == '"' && allChars[curIndex + 2] == '"') {
                quoteEnd = findIndexOfStringEnd(allChars, curIndex, "\"\"\"");
                if(quoteEnd == -1) return 0;
                tokenize(tLiteral, allChars, curIndex, quoteEnd + 3);
                curIndex = quoteEnd + 3;
            } else {
                quoteEnd = findIndexOfStringEnd(allChars, curIndex, "\"");
                if(quoteEnd == -1) return 0;
                tokenize(tLiteral, allChars, curIndex, quoteEnd + 1);
                curIndex = quoteEnd + 1;
            }
        }

        if (allChars[curIndex] == '\'') {
            int quoteEnd;
            if (allChars[curIndex + 1] == '\'' && allChars[curIndex + 2] == '\'') {
                quoteEnd = findIndexOfStringEnd(allChars, curIndex, "'''");
                if(quoteEnd == -1) return 0;
                tokenize(tLiteral, allChars, curIndex, quoteEnd + 3);
                curIndex = quoteEnd + 3;
            } else {
                quoteEnd = findIndexOfStringEnd(allChars, curIndex, "'");
                if(quoteEnd == -1) return 0;
                tokenize(tLiteral, allChars, curIndex, quoteEnd + 1);
                curIndex = quoteEnd + 1;
            }
        }

        if (!analyzeChar(allChars[curIndex])) return 0;
    }
    return 1;
}

int analyzeChar(char c) {
    if (isNesting) {
        if (c == ' ') {
            addToStr(temp, ' ');
            return 1;
        } else {
            if (addingNest) {
                if (strLength(temp) > nestStack->nest) {
                    nestStack = pushStack(nestStack, strLength(temp));
                    addingNest = 0;
                } else {
                    return 0;
                }
            } else {
                if(nestStack->nest != strLength(temp) && !recNestCheck()) return 0;
            }
            isNesting = 0;
            eraseStr(temp);
        }
    }

    if (c == '\n') {
        isNesting = 1;
    }

    switch (getType(c)) {
        case tDigit:
            if (tempClass == -1) {
                tempClass = tLiteral;
            }
            addToStr(temp, c);
            break;
        case tLetter:
            if (tempClass == tLiteral) {
                return 0;
            }
            if (tempClass == -1) {
                tempClass = tIdentifier;
            }
            addToStr(temp, c);
            break;
        case tSeparator:
            if (c == '-' && (strLength(temp) == 0 && getType(allChars[curIndex + 1]) == tDigit ||
                             (allChars[curIndex + 1] == '.' && getType(allChars[curIndex + 2]) == tDigit))) {
                addToStr(temp, c);
                tempClass = tLiteral;
                break;
            }

            if (c == '.' && getType(allChars[curIndex + 1]) == tDigit) {
                addToStr(temp, c);
                tempClass = tLiteral;
                break;
            }

            if (strLength(temp) > 0) {
                if (tempClass == tIdentifier && containsIn(strToCharArray(temp), keyWords, NELEMS(keyWords))) {
                    tempClass = tKeyWord;
                }
                tokenizeString(tempClass, temp);
                eraseStr(temp);
                tempClass = -1;
            }

            char btLexeme[4];
            btLexeme[0] = c;
            btLexeme[1] = allChars[curIndex + 1];
            btLexeme[2] = allChars[curIndex + 2];
            btLexeme[3] = 0;

            if (containsIn(btLexeme, operators, NELEMS(operators))) {
                tokenize(tOperator, allChars, curIndex, curIndex + 3);
                curIndex += 2;
                return 1;
            }

            btLexeme[2] = 0;

            if (containsIn(btLexeme, operators, NELEMS(operators))) {
                tokenize(tOperator, allChars, curIndex, curIndex + 2);
                curIndex++;
                return 1;
            }

            btLexeme[1] = 0;

            if (containsIn(btLexeme, operators, NELEMS(operators))) {
                tokenizeChar(tOperator, c);
                return 1;
            }

            if (c == ':') {
                addingNest = 1;
                tokenizeChar(tBlockOpener, c);
                break;
            }
            tokenizeChar(tDelimiter, c);
            break;
        default:
            printf("\nType not found\n");
            return 0;
    }
    return 1;
}

bool recNestCheck() {
    nestStack = popStack(nestStack);

    if(nestStack == NULL) return 0;

    tokenizeString(tBlockCloser, temp);
    if(nestStack->nest == strLength(temp)) {
        return 1;
    } else {
        return recNestCheck();
    }
}

int containsIn(const char *findable, const char *in[], int length) {
    for (int i = 0; i < length; i++) {
        if (strcmp(findable, in[i]) == 0) return 1;
    }
    return 0;
}

int findIndexOfStringEnd(char *string, int start, const char *quote) {
    for (int i = start + strlen(quote); i <= strlen(string); i++) {
        if (string[i] == '\\') {
            i++;
            continue;
        }
        if (compareSubstring(string, i, quote)) return i;
    }
    return -1;
}

int compareSubstring(const char *string, int start, const char *comparable) {
    if (strlen(string) - start < strlen(comparable)) return 0;

    int fin = start + strlen(comparable);
    int i = start;
    int j = 0;
    while (i < fin && j < strlen(comparable) && string[i] == comparable[j]) {
        j++;
        i++;
    }
    return j == strlen(comparable);
}

bool readFile() {
    char path[96];
    printf("Enter path to file:\n");
    scanf("%s", path);

    FILE *analyzable = fopen(path, "rt");
    if (analyzable) {
        fseek(analyzable, 0, SEEK_END);
        size = (unsigned long) ftell(analyzable);

        allChars = calloc(size + 1, sizeof(char));
        fseek(analyzable, 0, SEEK_SET);

        for (int i = 0; i <= size; i++) {
            allChars[i] = (char) getc(analyzable);

            if (allChars[i] == '\n') {
                size--;
            }
        }
        allChars[size--] = 0;

        fclose(analyzable);
        return 1;
    } else {
        printf("File not found.");
        return 0;
    }
}

int lexer() {
    if(!readFile()) return 0;
    int isLexAnalyzeSuccess = analyze();
    printStream();
    if (!isLexAnalyzeSuccess) {
        printf("Error occurred on index #%d (Line: %s).\n", curIndex, returnErrorLine());
    }
    return isLexAnalyzeSuccess;
}

char *returnErrorLine() {
    int prevNL = curIndex - 1;
    while (prevNL > 0 && allChars[prevNL] != '\n') prevNL--;
    int nextNL = curIndex;
    while (allChars[nextNL] != 0 && allChars[nextNL] != '\n') nextNL++;

    char * errorLine = calloc(nextNL - prevNL, sizeof(char ));
    for(int i = prevNL + 1; i < nextNL; i++) {
        errorLine[i - prevNL - 1] = allChars[i];
    }
    errorLine[nextNL - prevNL - 1] = 0;
    return errorLine;
}
