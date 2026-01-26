#ifndef PEBBLE_LEXER_H
#define PEBBLE_LEXER_H

#include "token.h"

// Initialize the lexer with source code
void lexer_init(const char *src);

// Get the next token
Token next_token(void);

#endif // PEBBLE_LEXER_H
