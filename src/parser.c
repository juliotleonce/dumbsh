#include "../headers/parser.h"

typedef struct TokenIterator {
    XArray_(Token) tokens;
    int position;
} TokenIterator;

static ASTNode *parse_sequence(TokenIterator *iterator);
static ASTNode *parse_pipeline(TokenIterator *iterator);
static ASTNode *parse_cmd(TokenIterator *iterator);
static XArray_(Redirection) parse_redirection(TokenIterator *iterator);
static ASTNode *node_new(ASTNodeType type);
static Token *peek(const TokenIterator *iterator);
static Token *advance(TokenIterator *iterator);
static bool match(const TokenIterator *iterator, TokenType type);

ASTNode *parse(XArray_(Token) tokens) {
    TokenIterator parser = { tokens, 0 };
    return parse_sequence(&parser);
}

ASTNode *parse_sequence(TokenIterator *iterator) {
    ASTNode *left = parse_pipeline(iterator);

    while (match(iterator, TOKEN_AND) || match(iterator, TOKEN_OR)) {
        ASTNodeType  type = peek(iterator)->type == TOKEN_AND ? AST_NODE_AND : AST_NODE_OR;
        advance(iterator);

        ASTNode *right = parse_pipeline(iterator);
        ASTNode *seq = node_new(type);

        seq->branch.left = left;
        seq->branch.right = right;
        left = seq;
    }

    return left;
}

ASTNode *parse_pipeline(TokenIterator *iterator) {
    ASTNode *left = parse_cmd(iterator);

    while (match(iterator, TOKEN_PIPE)) {
        advance(iterator);

        ASTNode *right = parse_cmd(iterator);
        ASTNode *pipe = node_new(AST_NODE_PIPELINE);

        pipe->branch.left = left;
        pipe->branch.right = right;
        left = pipe;
    }

    return left;
}

ASTNode *parse_cmd(TokenIterator *iterator) {
    ASTNode *cmd_node = node_new(AST_NODE_CMD);
    XString *cmd = peek(iterator)->value;

    XArray_(XString) argv = xarray_new(sizeof(XString));
    while (match(iterator, TOKEN_WORD)
        || match(iterator, TOKEN_SQUOTED)
        || match(iterator, TOKEN_DQUOTED)) {
        XString *arg = peek(iterator)->value;
        advance(iterator);
        xarray_push(argv, arg);
    }

    cmd_node->leaf.argv = argv;
    cmd_node->leaf.cmd = cmd;
    cmd_node->leaf.redirs = parse_redirection(iterator);

    return cmd_node;
}

static XArray_(Redirection) parse_redirection(TokenIterator *iterator) {
    XArray_(Redirection) redirs = xarray_new(sizeof(Redirection));

    while (match(iterator, TOKEN_REDIR_OUT)
        || match(iterator, TOKEN_REDIR_IN)
        || match(iterator, TOKEN_REDIR_APPEND)
    ) {
        TokenType type = peek(iterator)->type;
        advance(iterator);
        Token *word = peek(iterator);
        advance(iterator);

        if (word->type == TOKEN_WORD
            || word->type == TOKEN_SQUOTED
            || word->type == TOKEN_DQUOTED
        ) {
            Redirection redir;
            redir.path = word->value;
            if (type == TOKEN_REDIR_OUT) redir.type = REDIR_OUT;
            if (type == TOKEN_REDIR_IN) redir.type = REDIR_IN;
            if (type == TOKEN_REDIR_APPEND) redir.type = REDIR_APPEND;
            xarray_push(redirs, &redir);
        }
    }

    return redirs;
}

ASTNode *node_new(ASTNodeType type) {
    ASTNode *node = xmem_alloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

Token *peek(const TokenIterator *iterator) {
    return xarray_at(iterator->tokens, iterator->position);
}

Token *advance(TokenIterator *iterator) {
    iterator->position++;
    return xarray_at(iterator->tokens, iterator->position);
}

bool match(const TokenIterator *iterator, TokenType type) {
    return ((Token *)xarray_at(iterator->tokens, iterator->position))->type == type;
}
