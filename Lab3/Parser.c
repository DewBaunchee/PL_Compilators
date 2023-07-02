#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>
#include "Parser.h"
#include "TokenStream.h"
#include "stdbool.h"
#include <string.h>

Token current;

/********************************************
 * e - expression
 * ae - arithmetical expression
 * mae - multiply expression
 * pae - power expression
 * id - identifier, literal, function or other
 * eassign - enhanced assign
 * be - bool expression
 * abe - 'and' expression
 * nbe - 'not' expression
 * ce - compare expression
 * idB - identifier with brackets
 * idFB - identifier with figure brackets
 * idSB - identifier with square brackets
 */

bool term(enum classes type);

bool e();

bool ae();

bool mae();

bool pae();

bool be();

bool abe();

bool nbe();

bool ce();

bool id();

bool nestedId();

bool formatStr();

bool ifStatement();

bool elifStatement();

bool whileStatement();

bool forStatement();

bool importStatement();

bool tryStatement();

bool defDeclaration();

bool assign();

bool eassign();

bool returnStatement();

bool jumpStatement();

bool idB();
bool idFB();
bool idSB();

int exprNumber = 0;

// TERM
bool term(const enum classes type) {
    if (current == NULL) return 0;

    int temp = current->token;
    current = current->next;

    if (temp == tBlockOpener && temp == type) {
        current = current->next;
    }

    return temp == type;
}
//END TERM

// IDENTIFIER
bool nestedId() {
    Token recovery = current;

    if(current->next->token == tOpenBracket) return 0;

    if (term(tLiteral)) {
        return 1;
    }
    current = recovery;
    if (term(tIdentifier) && term(tDot) && nestedId()) {
        return 1;
    }
    current = recovery;
    if (term(tIdentifier) && term(tComma) && nestedId()) {
        return 1;
    }
    current = recovery;
    if (term(tLiteral) && term(tDot) && nestedId()) {
        return 1;
    }
    current = recovery;
    if (term(tLiteral) && term(tComma) && nestedId()) {
        return 1;
    }
    current = recovery;
    if (term(tIdentifier)) {

        return 1;
    }
    current = recovery;

    return 0;
}

bool formatStr() {
    if (current == NULL) return 0;

    bool isPercent = current->lexeme[0] == '%';
    current = current->next;

    return term(tLiteral) && isPercent && id();
}

bool idB() {
Token recovery = current;

    if(!hasUntil(current, tOpenBracket, tNewLine)) return 0;

    if(term(tIdentifier) && term(tOpenBracket)
       && term(tCloseBracket)) return 1;
    current = recovery;
    if(term(tIdentifier) && term(tOpenBracket)
       && ce() && term(tCloseBracket)) return 1;
    current = recovery;
    if(term(tOpenBracket) && term(tCloseBracket)) return 1;
    current = recovery;
    if(term(tOpenBracket) && ce() && term(tCloseBracket)) return 1;
    current = recovery;

    return 0;
}

bool idFB() {
    Token recovery = current;

    if(!hasUntil(current, tFigureOpenBracket, tNewLine)) return 0;

    if(term(tIdentifier) && term(tFigureOpenBracket)
       && term(tFigureCloseBracket)) return 1;
    current = recovery;
    if(term(tIdentifier) && term(tFigureOpenBracket)
       && ce() && term(tFigureCloseBracket)) return 1;
    current = recovery;
    if(term(tFigureOpenBracket) && term(tFigureCloseBracket)) return 1;
    current = recovery;
    if(term(tFigureOpenBracket) && ce() && term(tFigureCloseBracket)) return 1;
    current = recovery;

    return 0;
}

bool idSB() {
    Token recovery = current;

    if(!hasUntil(current, tSqOpenBracket, tNewLine)) return 0;

    if(term(tIdentifier) && term(tSqOpenBracket)
       && term(tSqCloseBracket)) return 1;
    current = recovery;
    if(term(tIdentifier) && term(tSqOpenBracket)
       && ce() && term(tSqCloseBracket)) return 1;
    current = recovery;
    if(term(tSqOpenBracket) && term(tSqCloseBracket)) return 1;
    current = recovery;
    if(term(tSqOpenBracket) && ce() && term(tSqCloseBracket)) return 1;
    current = recovery;
    if(term(tIdentifier)) return 1;
    current = recovery;
    if(term(tLiteral)) return 1;
    current = recovery;

    return 0;
}

bool id() {
    Token recovery = current;

    bool hasDot = hasUntil(current, tDot, tNewLine);
    bool hasComma = hasUntil(current, tComma, tNewLine);

    if (hasDot && idB() && term(tDot) && id()) {
        return 1;
    }
    current = recovery;
    if (hasComma && idB() && term(tComma) && id()) {
        return 1;
    }
    current = recovery;
    if (idB()) {
        return 1;
    }
    if (hasDot && idFB() && term(tDot) && id()) {
        return 1;
    }
    current = recovery;
    if (hasComma && idFB() && term(tComma) && id()) {
        return 1;
    }
    current = recovery;
    if (idFB()) {
        return 1;
    }
    if (hasDot && idSB() && term(tDot) && id()) {
        return 1;
    }
    current = recovery;
    if (hasComma && idSB() && term(tComma) && id()) {
        return 1;
    }
    current = recovery;
    if (idSB()) {
        return 1;
    }
    current = recovery;
    if (nestedId() && term(tAs) && nestedId()) {
        return 1;
    }
    current = recovery;
    if (hasDot && nestedId() && term(tDot) && id()) {
        return 1;
    }
    current = recovery;
    if (hasComma && nestedId() && term(tComma) && id()) {
        return 1;
    }
    current = recovery;
    if (formatStr()) {
        return 1;
    }
    current = recovery;
    if (nestedId()) {
        return 1;
    }
    current = recovery;

    return 0;
}
// END IDENTIFIER

// ARITHMETICAL EXPRESSION
bool pae() {
    Token recovery = current;

    if (be() && term(tOperator2) && pae()) {
        return 1;
    }
    current = recovery;
    if (be()) {
        return 1;
    }
    current = recovery;

    return 0;
}

bool mae() {
    Token recovery = current;

    if (pae() && term(tOperator1) && mae()) {
        return 1;
    }
    current = recovery;
    if (pae()) {
        return 1;
    }
    current = recovery;

    return 0;
}

bool ae() {
    Token recovery = current;

    if (mae() && term(tOperator0) && ae()) {
        return 1;
    }
    current = recovery;
    if (mae()) {
        return 1;
    }
    current = recovery;

    return 0;
}
// END ARITHMETICAL EXPRESSION

// COMPARE EXPRESSION

bool ce() {
    Token recovery = current;

    if (ae() && term(tCmpOperator) && ce()) {
        return 1;
    }
    current = recovery;
    if (ae()) {
        return 1;
    }
    current = recovery;

    return 0;
}

// END COMPARE EXPRESSION

// IF STATEMENT
bool elifStatement() {
    Token recovery = current;

    if (term(tElif)
        && ce() && term(tBlockOpener) && e() && term(tBlockCloser) && elifStatement())
        return 1;
    current = recovery;
    if (term(tElif)
        && ce() && term(tBlockOpener) && e() && term(tBlockCloser))
        return 1;
    current = recovery;

    return 0;
}

bool ifStatement() {
    Token recovery = current;

    if (term(tIf) && ce() && term(tBlockOpener) && e() && term(tBlockCloser) && elifStatement() && term(tElse)
        && term(tBlockOpener) && e() && term(tBlockCloser)) {
        return 1;
    }
    current = recovery;
    if (term(tIf) && ce() && term(tBlockOpener) && e() && term(tBlockCloser) && term(tElse)
        && term(tBlockOpener) && e() && term(tBlockCloser)) {
        return 1;
    }
    current = recovery;
    if (term(tIf) && ce() && term(tBlockOpener) && e() && term(tBlockCloser) && elifStatement()) {
        return 1;
    }
    current = recovery;
    if (term(tIf) && ce() && term(tBlockOpener) && e() && term(tBlockCloser)) {
        return 1;
    }
    current = recovery;

    return 0;
}
// END IF STATEMENT

// WHILE STATEMENT
bool whileStatement() {
    return term(tWhile) && ce() && term(tBlockOpener) && e() && term(tBlockCloser);
}
// END WHILE STATEMENT

// FOR STATEMENT
bool forStatement() {
    return term(tFor) && ce() && term(tBlockOpener) && e() && term(tBlockCloser);
}
// END FOR STATEMENT

// IMPORT STATEMENT
bool importStatement() {
    return term(tImport) && nestedId() && current->lexeme[0] == '\n';
}
// END IMPORT STATEMENT

// TRY STATEMENT
bool tryStatement() {
    Token recovery = current;

    if (term(tTry) && term(tBlockOpener) && e() && term(tBlockCloser) && term(tExcept) && id() && term(tBlockOpener) &&
        e() && term(tBlockCloser) &&
        term(tFinally) && id() && term(tBlockOpener) && e() && term(tBlockCloser)) {
        return 1;
    }
    current = recovery;
    if (term(tTry) && term(tBlockOpener) && e() && term(tBlockCloser) && term(tFinally) && id() && term(tBlockOpener) &&
        e() &&
        term(tBlockCloser)) {
        return 1;
    }
    current = recovery;
    if (term(tTry) && term(tBlockOpener) && e() && term(tBlockCloser) && term(tExcept) && id() && term(tBlockOpener) &&
        e() && term(tBlockCloser)) {
        return 1;
    }
    current = recovery;

    return 0;
}
// END TRY STATEMENT

// JUMP STATEMENTS
bool returnStatement() {
    Token recovery = current;

    if (term(tReturn) && ce()) {
        return 1;
    }
    current = recovery;
    if (term(tReturn)) {
        return 1;
    }
    current = recovery;

    return 0;
}

bool jumpStatement() {
    return term(tJump);
}
// END JUMP STATEMENTS

// DEF DECLARATION
bool defDeclaration() {
    return term(tDef) && term(tIdentifier) && term(tOpenBracket) && id() && term(tCloseBracket) && term(tBlockOpener) &&
           e() && term(tBlockCloser);
}
// END DEF DECLARATION

// BOOL EXPRESSION
bool be() {
    Token recovery = current;

    if (abe() && term(tOr) && be()) {
        return 1;
    }
    current = recovery;
    if (abe()) {
        return 1;
    }
    current = recovery;

    return 0;
}

bool abe() {
    Token recovery = current;

    if (nbe() && term(tAnd) && abe()) {
        return 1;
    }
    current = recovery;
    if (nbe()) {
        return 1;
    }
    current = recovery;

    return 0;
}

bool nbe() {
    Token recovery = current;

    if (term(tNot) && be()) {
        return 1;
    }
    current = recovery;
    if (id()) {
        return 1;
    }
    current = recovery;

    return 0;
}
// END BOOL EXPRESSION

// ASSIGN
bool assign() {
    Token recovery = current;

    if (id() && term(tAssign) && assign()) {
        return 1;
    }
    current = recovery;
    if (id() && term(tAssign) && ce()) {
        return 1;
    }
    current = recovery;

    return 0;
}
// END ASSIGN

// ENHANCED ASSIGN
bool eassign() {
    return id() && term(tEAssign) && ce();
}
// END ENHANCED ASSIGN

// MAIN EXPRESSION
bool e() {
    int check;
    while (current != NULL && current->token != tBlockCloser) {
        exprNumber++;
        if (exprNumber == 19);
        Token recovery = current;
        if (term(tNewLine)) continue;
        current = recovery;
        if (assign() && term(tNewLine)) {
            //printf("ASSIGN\n");
            continue;
        }
        current = recovery;
        if (eassign() && term(tNewLine)) {
            //printf("ENHANCED ASSIGN\n");
            continue;
        }
        current = recovery;
        if (ce() && term(tNewLine)) {
            //printf("ARITHMETIC EXPRESSION\n");
            continue;
        }
        current = recovery;
        if (ifStatement()) {
            //printf("IF STATEMENT\n");
            continue;
        }
        current = recovery;
        if (whileStatement()) {
            //printf("WHILE STATEMENT\n");
            continue;
        }
        current = recovery;
        if (forStatement()) {
            //printf("FOR STATEMENT\n");
            continue;
        }
        current = recovery;
        if (importStatement() && term(tNewLine)) {
            //printf("IMPORT STATEMENT\n");
            continue;
        }
        current = recovery;
        if (tryStatement()) {
            //printf("TRY STATEMENT\n");
            continue;
        }
        current = recovery;
        if (defDeclaration()) {
            //printf("DEF DECLARATION\n");
            continue;
        }
        current = recovery;
        if (returnStatement() && term(tNewLine)) {
            //printf("DEF DECLARATION\n");
            continue;
        }
        current = recovery;
        if (jumpStatement() && term(tNewLine)) {
            //printf("DEF DECLARATION\n");
            continue;
        }
        current = recovery;

        return 0;
    }
    return 1;
}
// END MAIN EXPRESSION

void printTrace() {
    for (int i = 0; i < 10; i++) {
        if (current == NULL) break;
        if (current->token == tNewLine) break;
        printf("----> %s : %s\n", classToStr(current->token), current->lexeme);
        current = current->next;
    }
}

void parser() {
    current = begin;
    pushStream(tNewLine, "\n");
    pushStream(tBlockCloser, " ");
    if (e()) {
        printf("Program matches Python grammar.\n");
    } else {
        printf(" ERROR (Expression #%d doesn't match Python grammar).\nPre error tokens:\n", exprNumber);
        printTrace();
    }
}

#endif