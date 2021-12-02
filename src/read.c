#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>

#include "read.h"

#define DEBUG false

extern int errno;

static size_t line;
static size_t col;

static char *data;

static bool valid;
static bool eof_found;
static bool is_newline;

static token_t curr_token;

void gettoken();
void next();
ast_t* read_form();

ast_t* read(char *input) {
    line = 1;
    col = 0;
    data = input;
    valid = true;
    eof_found = false;
    is_newline = false;

    next();
    ast_t* expr = read_form();

    if (valid) {
        return expr;
    } else {
        return NULL;
    }
}

void next() {
    if (!eof_found) {
        gettoken();

        if (curr_token.type == '\0') {
            eof_found = true;
        }

        if (DEBUG) {
            println_token(curr_token);
        }
    }
}

ast_t* read_list();
ast_t* read_atom();

ast_t* read_form() {
    if (curr_token.type == '(') {
        next();
        return read_list();
    } else {
        return read_atom();
    }
}

ast_t* read_list() {
    ast_t *node = malloc(sizeof(ast_t));
    node->type = LIST;

    size_t capacity = 0x10;
    node->children = malloc(capacity * sizeof(ast_t*));
    node->size = 0;

    while (curr_token.type != ')') {
        node->children[node->size++] = read_form();

        if (node->size == capacity) {
            capacity += capacity / 2;

            node->children = realloc(
                node->children,
                capacity * sizeof(ast_t*)
            );

            if (node->children == NULL) {
                perror("Error allocating ast");
            }
        }

        if (curr_token.type == '\0') {
            valid = false;
            printf("Syntax error: unbalanced parenthesis\n");
            break;
        }

        next();
    }

    if (node->size == 0) {
        free(node->children);
    }

    return node;
}

ast_t* read_atom() {
    ast_t *node = malloc(sizeof(ast_t));
    node->type = ATOM;
    node->token = curr_token;
    node->children = NULL;
    return node;
}

bool isreserved(char c);
bool isignorable(char c);

void nextchar();

char* getstring();
char* getsymbol();
char* getnumber();
char* getcomment();

token_e getnum_t(char *literal);

void gettoken() {
    while (isignorable(data[0])) {
        nextchar();
    }

    curr_token.line = line;
    curr_token.col = col;

    if (isreserved(data[0])) {
        if (data[0] == '~') {
            if (data[1] == '@') {
                curr_token.type = SPLICE_QUOTE;
                nextchar();
            } else { 
                curr_token.type = '~';
            }

            nextchar();

        } else {
            curr_token.type = data[0];
            nextchar();
        }

    } else if (data[0] == '\0') {
        curr_token.type = '\0';

    } else if (data[0] == ';') {
        curr_token.type = COMMENT;
        curr_token.string = getcomment();

    } else if (data[0] == '"') {
        curr_token.type = STRING;
        curr_token.string = getstring();

    } else if (isdigit(data[0])) {
        char *literal = getnumber();

        if (getnum_t(literal) == INTEGER) {
            curr_token.type = INTEGER;
            curr_token.integer = atoi(literal);

        } else {
            curr_token.type = REAL;
            curr_token.real = atof(literal);
        }

    } else {
        curr_token.type = SYMBOL;
        curr_token.string = getsymbol();
    }
}

bool isreserved(char c) {
    switch (c) {
    case '[':   return true;
    case ']':   return true;
    case '{':   return true;
    case '}':   return true;
    case '(':   return true;
    case ')':   return true;
    case '\'':  return true;
    case '`':   return true;
    case '~':   return true;
    case '^':   return true;
    case '@':   return true;
    default:    return false;
    }
}

bool isignorable(char c) {
    return isspace(c) || c == ',';
}

void nextchar() {
    if (is_newline) {
        is_newline = false;
        line++;
        col = 0;
    } else {
        col++;
    }

    if (data[0] == '\n' && isprint(data[1])) {
        is_newline = true;
    }

    data++;
}

void* check_capacity(void *buffer, size_t size, size_t capacity) {
    if (size == capacity) {
        capacity += capacity / 2;
        buffer = realloc(buffer, capacity);

        if (buffer == NULL) {
            perror("Error allocating string");
        }
    }

    return buffer;
}

char* getstring() {
    size_t capacity = 0x10, i = 0;
    char *string = malloc(capacity);
    bool open_string = true;
    
    string[i++] = data[0];

    while (open_string) {
        nextchar();

        switch (data[0]) {
        case '\\': {
            switch (data[1]) {
            case '"': {
                string[i++] = '"';
                nextchar();
            } break;

            case 't': {
                string[i++] = '\t';
                nextchar();
            } break;

            case 'n': {
                string[i++] = '\n';
                nextchar();
            } break;

            default:
                string[i++] = '\\';
                nextchar();
            }
        } break;

        case '"': {
            string[i++] = data[0];
            open_string = false;
            nextchar();
        } break;

        case '\0': {
            valid = false;
            printf("Syntax error: unclosed string\n");
            goto FAIL;

        } break;

        default:
            string[i++] = data[0];
        }

        string = check_capacity(string, i, capacity);
    }

    string[i] = '\0';
    return string;

FAIL:
    return NULL;
}

char* getsymbol() {
    size_t capacity = 0x10, i = 0;
    char *string = malloc(capacity);

    while (!isignorable(data[0]) && !isreserved(data[0])) {
        string[i++] = data[0];
        string = check_capacity(string, i, capacity);
        nextchar();
    }

    string[i] = '\0';
    return string;
}

char* getnumber() {
    size_t capacity = 0x10, i = 0;
    char *string = malloc(capacity);

    while (isdigit(data[0])) {
        string[i++] = data[0];
        string = check_capacity(string, i, capacity);
        nextchar();
    }

    if (data[0] == '.') {
        string[i++] = data[0];
        string = check_capacity(string, i, capacity);
        nextchar();
    }

    if (isdigit(data[0])) {
        while (isdigit(data[0])) {
            string[i++] = data[0];
            string = check_capacity(string, i, capacity);
            nextchar();
        }
    }

    string[i] = '\0';
    return string;
}

char* getcomment() {
    size_t capacity = 0x10, i = 0;
    char *string = malloc(capacity);

    nextchar();

    while (data[0] != '\n') {
        string[i++] = data[0];
        string = check_capacity(string, i, capacity);
        nextchar();
    }

    string[i] = '\0';
    return string;
}

token_e getnum_t(char *literal) {
    while (*literal != '\0') {
        if (*(literal++) == '.') {
            return REAL;
        }
    }

    return INTEGER;
}
