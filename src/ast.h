#ifndef AST_H
#define AST_H

#include <stdlib.h>

#include "token.h"

typedef enum {
    LIST,
    ATOM,
} ast_e;

typedef struct ast_t {
    ast_e type;
    token_t token;
    struct ast_t **children;
    size_t size;
} ast_t;

#endif
