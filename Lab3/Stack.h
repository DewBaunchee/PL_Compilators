#ifndef LAB3_STACK_H
#define LAB3_STACK_H

typedef struct sLink {
    int nest;
    struct sLink* down;
} * Stack;

Stack pushStack(Stack top, int data);
Stack popStack(Stack top);
int isStackEmpty(Stack top);
void printStack(Stack top);

#endif
