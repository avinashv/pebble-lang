#ifndef PEBBLE_TOKEN_H
#define PEBBLE_TOKEN_H

#include "limits.h"

// Token type
typedef enum {
    TOK_EOF,     // End of file
    TOK_NEWLINE, // End of line
    TOK_WORD,    // Identifier or keyword
    TOK_NUMBER,  // Integer
    TOK_STRING,  // String literal
} TokenType;

// Token structure
typedef struct {
    TokenType type;              // Type of token
    char text[MAX_IDENT_LEN];    // Text of the token
    int line;                    // Line number of the token
    int col;                     // Column number of the token
} Token;

// Convert token type to string
const char *token_type_name(TokenType type);

#endif // PEBBLE_TOKEN_H
