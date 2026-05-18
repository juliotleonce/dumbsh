#include "../headers/tokenizer.h"

#include <ctype.h>

typedef struct Tokenizer {
    XString *source;
    int position;
} Tokenizer;

Token *get_next_token(Tokenizer *tokenizer);
Token *read_word(Tokenizer *tokenizer);
Token *read_quoted(Tokenizer *tokenizer, char quote);
Token *read_operator(Tokenizer *tokenizer);
Token *token_new(TokenType type, XString *value);
void skip_whitespace(Tokenizer *tokenizer);
bool is_word_char(char c);
char peek(const Tokenizer *tokenizer);
char advance(Tokenizer *tokenizer);

XResult(XArray_(Token)) tokenize(XString *source) {
    XArray_(Token) tokens = xarray_new(sizeof(Token));
    Tokenizer tokenizer = { source,  0 };
    Token *token;

    while ((token = get_next_token(&tokenizer))->type != TOKEN_EOF) {
        xarray_push(tokens, token);
        xmem_free(token);
    }

    xarray_push(tokens, token);
    xmem_free(token);

    return OK(XArray_(Token), tokens);
}


Token *get_next_token(Tokenizer *tokenizer) {
    skip_whitespace(tokenizer);

    if (peek(tokenizer) == '\0') {
        advance(tokenizer);
        return token_new(TOKEN_EOF, NULL);
    }

    if (peek(tokenizer) == '"' || peek(tokenizer) == '\'') {
        return read_quoted(tokenizer, peek(tokenizer));
    }

    if (is_word_char(peek(tokenizer))) {
        return read_word(tokenizer);
    }

    return read_operator(tokenizer);
}

Token *read_word(Tokenizer *tokenizer) {
    unsigned start = tokenizer->position;
    while (is_word_char(peek(tokenizer)) && peek(tokenizer) != '\0') {
        advance(tokenizer);
    }

    XString *value = xstring_substring(tokenizer->source, start, tokenizer->position);
    return token_new(TOKEN_WORD, value);
}

Token *read_quoted(Tokenizer *tokenizer, char quote) {
    advance(tokenizer);
    unsigned start = tokenizer->position;
    while (peek(tokenizer) != quote && peek(tokenizer) != '\0') {
        advance(tokenizer);
    }

    XString *value = xstring_substring(tokenizer->source, start, tokenizer->position);
    TokenType token_type= quote == '"' ? TOKEN_SQUOTED : TOKEN_DQUOTED;
    advance(tokenizer);
    return token_new(token_type, value);
}

Token *read_operator(Tokenizer *tokenizer) {
    if (peek(tokenizer) == '&') {
        advance(tokenizer);
        if (peek(tokenizer) != '&') return token_new(TOKEN_AMPERSAND, NULL);
        advance(tokenizer);
        return token_new(TOKEN_AND, NULL);
    }

    if (peek(tokenizer) == '|') {
        advance(tokenizer);
        if (peek(tokenizer) != '|') return token_new(TOKEN_PIPE, NULL);
        advance(tokenizer);
        return token_new(TOKEN_OR, NULL);
    }

    return token_new(TOKEN_UNKNOWN, NULL);
}

Token *token_new(TokenType type, XString *value) {
    Token *token = xmem_alloc(sizeof(Token));
    token->type = type;
    token->value = value;
    return token;
}

void skip_whitespace(Tokenizer *tokenizer) {
    while (isspace(peek(tokenizer)) && peek(tokenizer) != '\0') {
        tokenizer->position++;
    }
}

bool inline is_word_char(char c) {
    return
        isalnum(c) ||
        c == '_' ||
        c == '-' ||
        c == '$' ||
        c == '.' ||
        c == '/';
}

char peek(const Tokenizer *tokenizer) {
    return xstring_at(tokenizer->source, tokenizer->position);
}

char advance(Tokenizer *tokenizer) {
    tokenizer->position++;
    return xstring_at(tokenizer->source, tokenizer->position-1);
}


