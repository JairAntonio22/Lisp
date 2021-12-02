#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>

typedef enum {
    SYMBOL = 0x100,
    NUMBER,
    STRING,
    COMMENT,
    SPLICE_QUOTE,
} token_e;

typedef struct {
    token_e type;
    char * literal;
    size_t line, col;
} token_t;

void print_token(token_t token);

#endif
