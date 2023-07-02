#include "stdbool.h"
#include "Lexer.h"
#include "Parser.h"
#include <stdio.h>

bool hub();

int main() {
    while (hub()) {
        if (lexer()) {
            parser();
        }
    }
    printf("Done!");
    return 0;
}

bool hub() {
    printf("Have another program? [Y/N]\n");
    fflush(stdin);
    char c;
    scanf("%c", &c);
    return c == 'Y' || c == 'y';
}

