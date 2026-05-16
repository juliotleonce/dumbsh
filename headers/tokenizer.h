#ifndef DUMPSH_TOKENIZER_H
#define DUMPSH_TOKENIZER_H
#include <xstdlib/xstdlib.h>

typedef enum TokenType {
    TOKEN_KEYWORD,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_AMPERSAND,
    TOKEN_WORD,
    TOKEN_DQUOTED,
    TOKEN_SQUOTED,
    TOKEN_UNKNOWN,
    TOKEN_EOF,
} TokenType;

typedef struct Tokenizer Tokenizer;

typedef struct Token {
    XString *value;
    TokenType type;
} Token;

DEFINE_XARRAY_OF(Token)
DEFINE_XRESULT_OF(XArray_(Token))

XResult(XArray_(Token)) tokenize(XString *source);

#endif
