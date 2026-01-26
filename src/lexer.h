#ifndef PEBBLE_LEXER_H
#define PEBBLE_LEXER_H

#include "token.h"

// Lexer state
typedef struct {
    const char *source; // The entire source code
    int pos;            // Current position (byte offset)
    int line;           // Current line number
    int col;            // Current column number
} LexerState;

// Initialize the lexer with source code
void lexer_init(LexerState *state, const char *src);

// Get the next token
Token next_token(LexerState *state);

#endif // PEBBLE_LEXER_H
