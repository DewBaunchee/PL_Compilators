#include <stdio.h>

int getSymbolType(int next);
void checkNumber();

int main() {
    do {
        checkNumber();
    } while (1);
    return 0;
}

void checkNumber() {
    int isFinalState[7] = {0, 1, 0, 0, 0, 1, 1};
    int transitions[7][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 2, 3, 4, 0, 0, 0, 0},
            {0, 2, 3, 4, 4, 5, 5, 5},
            {0, 3, 3, 4, 4, 4, 6, 6},
            {0, 4, 4, 4, 4, 4, 0, 6},
            {1, 4, 4, 4, 4, 4, 0, 6},
            {1, 0, 0, 0, 0, 0, 0, 0}
    };

    printf("Enter 2-, 8- or 16-radix number: \n");
    int next;
    int state = 1;
    while((next = getchar()) != '\n') {
        state = transitions[state][getSymbolType(next)];
    }
    if(isFinalState[state]) {
        printf("Correct!\n");
    } else {
        printf("Error!\n");
    }
}

int getSymbolType(int next) {
    switch (next) {
        case '0':
        case '1': return 1;
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7': return 2;
        case '8':
        case '9': return 3;
        case 'A':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'a':
        case 'c':
        case 'd':
        case 'e':
        case 'f': return 4;
        case 'B':
        case 'b': return 5;
        case 'O':
        case 'o': return 6;
        case 'H':
        case 'h': return 7;
        default: return 0;
    }
}
