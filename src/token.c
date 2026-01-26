#include "token.h"

// Convert token type to string
const char *token_type_name(TokenType type) {
    switch (type) {
        case TOK_EOF:
            return "EOF";
        case TOK_NEWLINE:
            return "NEWLINE";
        case TOK_WORD:
            return "WORD";
        case TOK_NUMBER:
            return "NUMBER";
        case TOK_STRING:
            return "STRING";
        default:
            return "UNKNOWN";
    }
}
