#include "Stack.h"
#include "stdlib.h"
#include "stdio.h"

Stack pushStack(Stack top, int data) {
    Stack newLink = calloc(1, sizeof(struct sLink));
    newLink->down = top;
    newLink->nest = data;
    return newLink;
}

Stack popStack(Stack top) {
    return top->down;
}

int isStackEmpty(Stack top) {
    return top == NULL;
}

void printStack(Stack top) {
    if (isStackEmpty(top)) {
        printf("EMPTY STACK\n");
    } else {
        printf("TOP - > %d\n", top->nest);
        top = top->down;
        while (!isStackEmpty(top)) {
            printf("%d\n", top->nest);
            top = top->down;
        }
    }
}
