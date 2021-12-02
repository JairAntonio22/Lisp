#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "read.h"
#include "print.h"

extern int errno;

void eval(char *input) {
}

void rep(char *input) {
    ast_t* tree = read(input);
    // eval(input);
    print(tree);
}

int main(void) {
    char *buffer;
    size_t buffsize = 256;

    buffer = malloc(buffsize);

    if (buffer == NULL) {
        perror("Error allocating buffer");
    }

    printf("> ");

    while (getline(&buffer, &buffsize, stdin) > 0) {
        rep(buffer);
        printf("> ");
    }
}
