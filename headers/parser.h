#ifndef DUMBSH_PARSER_H
#define DUMBSH_PARSER_H

#include "tokenizer.h"

DEFINE_XARRAY_OF(XString)
DEFINE_XARRAY_OF(Redirection)

typedef struct TokenIterator TokenIterator;
typedef struct ASTNode ASTNode;

typedef enum RedirectionType {
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
} RedirectionType;

typedef struct Redirection {
    XString *path;
    RedirectionType type;
} Redirection;

typedef enum ASTNodeType {
  AST_NODE_CMD,
  AST_NODE_PIPELINE,
  AST_NODE_AND,
  AST_NODE_OR,
} ASTNodeType;

typedef struct ASTNodeLeaf {
    ASTNodeType type;
    XString *cmd;
    XArray_(XString) argv;
    XArray_(Redirection) redirs;
} ASTNodeLeaf;

typedef struct ASTNodeBranch {
    ASTNodeType type;
    ASTNode *left;
    ASTNode *right;
} ASTNodeBranch;

typedef struct ASTNode {
    ASTNodeType type;
    union {
        ASTNodeLeaf leaf;
        ASTNodeBranch branch;
    };
} ASTNode;

ASTNode *parse(XArray_(Token) tokens);

#endif