#include "TokenStream.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum classes refineClass(enum classes classes, char *lexeme);

void pushStream(enum classes token, const char *lexeme) {
    if (begin == NULL) {
        begin = end = calloc(1, sizeof(struct sToken));
        begin->lexeme = calloc(strlen(lexeme) + 1, sizeof(char));
        strcpy(begin->lexeme, lexeme);

        begin->token = token;
        begin->next = NULL;
    } else {
        end->next = malloc(sizeof(struct sToken));
        end = end->next;

        end->lexeme = calloc(strlen(lexeme) + 1, sizeof(char));
        strcpy(end->lexeme, lexeme);

        end->token = token;
        end->next = NULL;
    }
}

Token popStream() {
    Token returnable = begin;
    if (begin == end) {
        begin = end = NULL;
    } else {
        begin = returnable->next;
    }

    returnable->next = NULL;
    return returnable;
}

int hasNext() {
    return end != NULL;
}

enum classes refineClass(enum classes type, char *lexeme) {
    if(type == tKeyWord) {
        if(strcmp(lexeme, "True") == 0 || strcmp(lexeme, "False") == 0 || strcmp(lexeme, "None") == 0) return tLiteral;
        if(strcmp(lexeme, "break") == 0 || strcmp(lexeme, "continue") == 0) return tJump;
        if(strcmp(lexeme, "try") == 0) return tTry;
        if(strcmp(lexeme, "except") == 0) return tExcept;
        if(strcmp(lexeme, "finally") == 0) return tFinally;
        if(strcmp(lexeme, "for") == 0) return tFor;
        if(strcmp(lexeme, "while") == 0) return tWhile;
        if(strcmp(lexeme, "if") == 0) return tIf;
        if(strcmp(lexeme, "else") == 0) return tElse;
        if(strcmp(lexeme, "elif") == 0) return tElif;
        if(strcmp(lexeme, "in") == 0) return tCmpOperator;
        if(strcmp(lexeme, "return") == 0) return tReturn;
        if(strcmp(lexeme, "as") == 0) return tAs;
        if(strcmp(lexeme, "import") == 0) return tImport;
        if(strcmp(lexeme, "not") == 0) return tNot;
        if(strcmp(lexeme, "or") == 0) return tOr;
        if(strcmp(lexeme, "and") == 0) return tAnd;
        if(strcmp(lexeme, "def") == 0) return tDef;
    }
    if(type == tOperator) {
        if(strcmp(lexeme, "+") == 0 || strcmp(lexeme, "-") == 0) return tOperator0;
        if(strcmp(lexeme, "*") == 0 || strcmp(lexeme, "/") == 0 || strcmp(lexeme, "%") == 0) return tOperator1;
        if(strcmp(lexeme, "**") == 0 || strcmp(lexeme, "//") == 0) return tOperator2;
        if(strcmp(lexeme, ">") == 0 || strcmp(lexeme, "<") == 0
        || strcmp(lexeme, "<=") == 0 || strcmp(lexeme, ">=") == 0
        || strcmp(lexeme, "==") == 0 || strcmp(lexeme, "!=") == 0) return tCmpOperator;
        if(lexeme[0] == '=') return tAssign;
        if(lexeme[strlen(lexeme) - 1] == '=') return tEAssign;
    }
    if(type == tDelimiter) {
        if(lexeme[0] == '(') return tOpenBracket;
        if(lexeme[0] == ')') return tCloseBracket;
        if(lexeme[0] == '[') return tSqOpenBracket;
        if(lexeme[0] == ']') return tSqCloseBracket;
        if(lexeme[0] == '{') return tFigureOpenBracket;
        if(lexeme[0] == '}') return tFigureCloseBracket;
        if(lexeme[0] == ',') return tComma;
        if(lexeme[0] == '.') return tDot;
        if(lexeme[0] == '\n') return tNewLine;
    }
    return type;
}

void tokenize(enum classes type, const char *src, int start, int fin) {
    char *newLexeme = calloc(fin - start + 1, sizeof(char));
    for (int i = start; i < fin; i++)
        newLexeme[i - start] = src[i];
    type = refineClass(type, newLexeme);
    pushStream(type, newLexeme);
}

void tokenizeString(enum classes type, String src) {
    char *newLexeme = calloc(strLength(src) + 1, sizeof(char));
    for (int i = 0; i < strLength(src); i++)
        newLexeme[i] = charAtStr(src, i);
    type = refineClass(type, newLexeme);
    pushStream(type, newLexeme);
}

void tokenizeChar(enum classes type, char src) {
    if(src == ' ') return;
    char *newLexeme = calloc(2, sizeof(char));
    newLexeme[0] = src;
    type = refineClass(type, newLexeme);
    pushStream(type, newLexeme);
}

int hasUntil(Token start, enum classes findable, enum classes until) {
    while (start != NULL && start->token != findable && start->token != until) start = start->next;
    return start != NULL && start->token == findable;
}

const char *classToStr(enum classes cl) {
    switch (cl) {
        case tIdentifier:
            return "Identifier";
        case tOperator:
            return "Operator";
        case tKeyWord:
            return "Key word";
        case tDelimiter:
            return "Delimiter";
        case tBlockOpener:
            return "Block opener";
        case tBlockCloser:
            return "Block closer";
        case tFigureOpenBracket:
            return "Figure open bracket";
        case tFigureCloseBracket:
            return "Figure close bracket";
        case tNest:
            return "Nest";
        case tAs:
            return "As";
        case tLiteral:
            return "Literal";
        case tBoolOperation:
            return "Bool operation";
        case tDef:
            return "Function declaration";
        case tIf:
            return "If";
        case tElse:
            return "Else";
        case tElif:
            return "Elif";
        case tTry:
            return "Try";
        case tExcept:
            return "Except";
        case tFinally:
            return "Finally";
        case tFor:
            return "For";
        case tWhile:
            return "While";
        case tImport:
            return "Import";
        case tIn:
            return "In";
        case tJump:
            return "Jump";
        case tReturn:
            return "Return";
        case tOperator0:
            return "Plus/Minus";
        case tOperator1:
            return "Mul/Dev/Mod";
        case tOperator2:
            return "Div/Pow";
        case tAssign:
            return "Assign";
        case tEAssign:
            return "Enhanced assign";
        case tOpenBracket:
            return "Open bracket";
        case tCloseBracket:
            return "Close bracket";
        case tSqOpenBracket:
            return "Square open bracket";
        case tSqCloseBracket:
            return "Square close bracket";
        case tComma:
            return "Comma";
        case tDot:
            return "Dot";
        case tNewLine:
            return "New line";
        case tNot:
            return "Not";
        case tAnd:
            return "And";
        case tOr:
            return "Or";
        case tCmpOperator:
            return "Compare operator";
        default:
            return "UNKNOWN";
    }
}

const char *lexemeToStr(const char *lexeme) {
    if (lexeme[0] == ' ') return "[SPACE]";
    if (strcmp(lexeme, "\t") == 0) return "[TAB]";
    if (strcmp(lexeme, "\n") == 0) return "[NEW LINE]";
    return lexeme;
}

void printStream() {
    Token current = begin;
    if (current == NULL) {
        printf("EMPTY\n");
    } else {
        printf("+----------------------------------\n");
        while (current->next != NULL) {
            if (current->token == tBlockCloser) {
                printf("%s : %llux[SPACE] \n", classToStr(current->token), strlen(current->lexeme));
            } else {
                printf("%s : %s \n", classToStr(current->token), lexemeToStr(current->lexeme));
            }
            current = current->next;
        }
        if (current->token == tBlockCloser) {
            printf("%s : %llux[SPACE] \n", classToStr(current->token), strlen(current->lexeme));
        } else {
            printf("%s : %s \n", classToStr(current->token), lexemeToStr(current->lexeme));
        }
        printf("+----------------------------------\n");
    }
}