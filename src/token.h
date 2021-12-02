#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    SYMBOL = 0x100,
    INTEGER,
    REAL,
    STRING,
    COMMENT,
    SPLICE_QUOTE,
} token_e;

typedef struct {
    token_e type;
    union {
        char *string;
        int integer;
        float real;
    };
    size_t line, col;
} token_t;

void print_token(token_t token);
void println_token(token_t token);

#endif
