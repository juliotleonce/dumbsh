#include <stdio.h>
#include <stdlib.h>
#include <xstdlib/xresult.h>

#include "../headers/parser.h"
#include "../headers/tokenizer.h"

void print_token(const Token *token);

int main(void) {
    XString *source = xstring_new("dumpsh \"hello world\" | echo hello && echo 'hello mars'");
    XArray_(Token) tokens = UNWRAP(tokenize(source));
    ASTNode *ast = parse(tokens);

    for (int i = 0; i < tokens->length; i++) {
        Token *token = xarray_at(tokens, i);
        print_token(token);
    }

    return 0;
}

void print_token(const Token *token) {
    char *type;
    switch (token->type) {
        case TOKEN_AND:
            type = "AND";
            break;
        case TOKEN_WORD:
            type = "WORD";
            break;
        case TOKEN_DQUOTED:
            type = "DQUOTED";
            break;
        case TOKEN_SQUOTED:
            type = "SQUOTED";
            break;
        case TOKEN_EOF:
            type = "EOF";
            break;
        case TOKEN_KEYWORD:
            type = "KEYWORD";
            break;

        default:
            type = "UNKNOWN";
            break;
    }

    char *value;
    if (token->value == NULL) value = "NULL";
    else value = token->value->c_str;

    printf("%s: %s\n", type, value);
}