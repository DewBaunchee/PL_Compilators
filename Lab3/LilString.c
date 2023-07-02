#include "LilString.h"
#include "stdlib.h"
#include "stdio.h"

void addToStr(String str, char c) {
    String current = str;
    if (current->symbol == 0) {
        current->next = 0;
        current->symbol = c;
    } else {
        while (current->next != 0) current = current->next;

        current->next = calloc(1, sizeof(struct sString));
        current->next->symbol = c;
    }
}

void eraseStr(String str) {
    str->symbol = 0;
    String current = str->next;

    while (current != 0) {
        String temp = current;
        current = current->next;
        free(temp);
    }
    str->next = 0;
}

int strLength(String str) {
    if(str->symbol == 0) return 0;

    int l = 1;
    while (str->next != 0) {
        l++;
        str = str->next;
    }
    return l;
}

char charAtStr(String str, int index) {
    if(index < 0 || index >= strLength(str)) return 0;

    while (index > 0) {
        index--;
        str = str->next;
    }

    return str->symbol;
}

char *strToCharArray(String str) {
    char * dest = calloc(strLength(str) + 1, sizeof (char));

    int i = 0;
    while (str != 0) {
        dest[i] = str->symbol;
        str = str->next;
        i++;
    }
    return dest;
}

void printString(String str) {
    String current = str;
    while (current != 0) {
        printf("%c", current->symbol);
        current = current->next;
    }
}