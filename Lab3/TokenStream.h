#ifndef TOKENSTREAM_H
#define TOKENSTREAM_H

#include "LilString.h"

enum classes {
    tIdentifier, tKeyWord, tOperator, tDelimiter, tBlockOpener, tBlockCloser, tNest, tNot, tOr, tAnd,
    tLiteral, tJump, tDef, tIf, tElse, tElif, tTry, tExcept, tFinally, tFor, tImport,
    tIn, tReturn, tWhile, tBoolOperation, tAs, tOperator0, tOperator1, tOperator2, tComma, tDot, tNewLine,
    tAssign, tEAssign, tOpenBracket, tCloseBracket, tSqOpenBracket, tSqCloseBracket, tCmpOperator,
    tFigureOpenBracket, tFigureCloseBracket
};

typedef struct sToken {
    enum classes token;
    char * lexeme;
    struct sToken *next;
} *Token;

Token begin, end;

void pushStream(enum classes token, const char* lexeme);
Token popStream();
int hasNext();
void printStream();
void tokenize(enum classes, const char *, int, int);
void tokenizeString(enum classes, String);
void tokenizeChar(enum classes, char);
enum classes refineClass(enum classes classes, char *lexeme);
const char *classToStr(enum classes cl);
int hasUntil(Token start, enum classes findable, enum classes until);

#endif
