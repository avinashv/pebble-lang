#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "limits.h"

// Initialize the lexer with source code
void lexer_init(LexerState *state, const char *src) {
    state->source = src;
    state->pos = 0;
    state->line = 1;
    state->col = 1;
}

// Look at the current character without consuming it
static char peek(LexerState *state) {
    return state->source[state->pos];
}

// Consume the current character and advance
static char advance(LexerState *state) {
    // Get the current character
    char ch = state->source[state->pos];

    // Advance the position
    state->pos++;
    
    // Update the line and column numbers
    if (ch == '\n') {
        state->line++;
        state->col = 1;
    } else {
        state->col++;
    }

    // Return the character
    return ch;
}

// Are we at the end of the source?
static int at_end(LexerState *state) {
    return state->source[state->pos] == '\0';
}

// Is this character a letter or underscore?
static int is_alpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

// Is this character a digit?
static int is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

// Is this character part of a word?
static int is_alpha_numeric(char ch) {
    return is_alpha(ch) || is_digit(ch);
}

// Is this character whitespace (but not newline)?
static int is_whitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\f';
}

// Is this character a newline?
static int is_newline(char ch) {
    return ch == '\n';
}

// Skip whitespace
static void skip_whitespace(LexerState *state) {
    while (is_whitespace(peek(state))) {
        advance(state);
    }
}

// Skip comments (from "#" to end of line)
static void skip_comment(LexerState *state) {
    while (!at_end(state) && peek(state) != '\n') {
        advance(state);
    }
}

// Get the next token
Token next_token(LexerState *state) {
    Token tok;
    tok.text[0] = '\0'; // Initialize the text to an empty string

    skip_whitespace(state);

    // Set the line and column numbers
    tok.line = state->line;
    tok.col = state->col;

    // Handle end of file
    if (at_end(state)) {
        tok.type = TOK_EOF;
        return tok;
    }

    char ch = peek(state);

    // Handle comments
    if (ch == '#') {
        skip_comment(state);
        // Return the next token recursively
        return next_token(state);
    }

    // Handle newlines
    if (is_newline(ch)) {
        // Set the token type and text
        tok.type = TOK_NEWLINE;
        tok.text[0] = '\n';
        tok.text[1] = '\0';
        
        advance(state);
        return tok;
    }

    // Handle words (identifiers)
    if (is_alpha(ch)) {
        tok.type = TOK_WORD;

        // Add characters of the identifier to the token text
        int i = 0;
        while (is_alpha_numeric(peek(state)) && i < MAX_IDENT_LEN - 1) {
            tok.text[i++] = advance(state);
        }
        tok.text[i] = '\0';
        return tok;
    }

    // Handle numbers (integers and floats)
    if (is_digit(ch)) {
        tok.type = TOK_NUMBER;

        // Add characters of the number to the token text
        int i = 0;
        while (is_digit(peek(state)) && i < MAX_IDENT_LEN - 1) {
            tok.text[i++] = advance(state);
        }

        // Handle decimal point for floats
        if (peek(state) == '.' && i < MAX_IDENT_LEN - 1) {
            tok.text[i++] = advance(state);  // Consume the '.'
            while (is_digit(peek(state)) && i < MAX_IDENT_LEN - 1) {
                tok.text[i++] = advance(state);
            }
        }

        tok.text[i] = '\0';
        return tok;
    }

    // Handle strings
    if (ch == '"') {
        tok.type = TOK_STRING;
        advance(state);  // Skip opening quote

        // Add characters of the string to the token text
        int i = 0;
        while (peek(state) != '"' && peek(state) != '\n' && !at_end(state) && i < MAX_IDENT_LEN - 1) {
            tok.text[i++] = advance(state);
        }
        tok.text[i] = '\0';

        if (peek(state) == '"') {
            advance(state);  // Skip closing quote
        } else {
            printf("Error at line %d, col %d: unterminated string\n",
                   tok.line, tok.col);
            exit(1);  // Exit the program with an error
        }
        return tok;
    }

    // Handle unknown character
    printf("Error at line %d, col %d: unexpected '%c'\n", state->line, state->col, ch);
    exit(1);  // Exit the program with an error
}
