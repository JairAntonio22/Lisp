#include <stdio.h>

#include "token.h"

void print_token(token_t token) {
    if (token.type < 0x100) {
        if (token.type == '\0') {
            printf("EOF\n");
        } else {
            printf("%c\n", token.type);
        }

    } else {
        printf("%s\n", token.literal);
    }
}

