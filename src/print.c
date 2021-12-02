#include <stdio.h>

#include "print.h"

void print_aux(ast_t *node);

void print(ast_t *node) {
    if (node) {
        print_aux(node);
        printf("\n");
    }
}

void print_aux(ast_t *node) {
    switch (node->type) {
    case LIST: {
        size_t i;

        printf("(");

        if (node->size > 0) {
            for (i = 0; i < node->size - 1; i++) {
                print_aux(node->children[i]);
                printf(" ");
            }

            print_aux(node->children[i]);
        }

        printf(")");
    } break;

    case ATOM: {
        print_token(node->token);
    } break;

    default:
        printf("Fatal error: Token type not recognized\n");
        exit(EXIT_FAILURE);
    }
}
