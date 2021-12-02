#include <stdio.h>

#include "token.h"

void print_token(token_t token) {
    if (token.type == '\0') {
        printf("EOF");

    } else if (token.type < 0x100) {
        printf("%c", token.type);

    } else if (token.type == INTEGER) {
        printf("%d", token.integer);

    } else if (token.type == REAL) {
        printf("%g", token.real);

    } else {
        printf("%s", token.string);
    }
}

void println_token(token_t token) {
    if (token.type == '\0') {
        printf("EOF\n");

    } else if (token.type < 0x100) {
        printf("%c\n", token.type);

    } else if (token.type == INTEGER) {
        printf("%d\n", token.integer);

    } else if (token.type == REAL) {
        printf("%g\n", token.real);

    } else {
        printf("%s\n", token.string);
    }
}
