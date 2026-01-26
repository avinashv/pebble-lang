#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

// Lexer state
static const char *source; // The entire source code
static int pos;            // Current position (byte offset)
static int line;           // Current line number
static int col;            // Current column number

// Initialize the lexer with source code
void lexer_init(const char *src) {
    source = src;
    pos = 0;
    line = 1;
    col = 1;
}

// Look at the current character without consuming it
static char peek(void) {
    return source[pos];
}

// Consume the current character and advance
static char advance(void) {
    // Get the current character
    char ch = source[pos];

    // Advance the position
    pos++;
    
    // Update the line and column numbers
    if (ch == '\n') {
        line++;
        col = 1;
    } else {
        col++;
    }

    // Return the character
    return ch;
}

// Are we at the end of the source?
static int at_end(void) {
    return source[pos] == '\0';
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
static void skip_whitespace(void) {
    while (is_whitespace(peek())) {
        advance();
    }
}

// Skip comments (from "#" to end of line)
static void skip_comment(void) {
    while (!at_end() && peek() != '\n') {
        advance();
    }
}

// Get the next token
Token next_token(void) {
    Token tok;
    tok.text[0] = '\0'; // Initialize the text to an empty string

    skip_whitespace();

    // Set the line and column numbers
    tok.line = line;
    tok.col = col;

    // Handle end of file
    if (at_end()) {
        tok.type = TOK_EOF;
        return tok;
    }

    char ch = peek();

    // Handle comments
    if (ch == '#') {
        skip_comment();
        // Return the next token recursively
        return next_token();
    }

    // Handle newlines
    if (is_newline(ch)) {
        // Set the token type and text
        tok.type = TOK_NEWLINE;
        tok.text[0] = '\n';
        tok.text[1] = '\0';
        
        advance();
        return tok;
    }

    // Handle words (identifiers)
    if (is_alpha(ch)) {
        tok.type = TOK_WORD;

        // Add characters of the identifier to the token text
        int i = 0;
        while (is_alpha_numeric(peek()) && i < 63) {
            tok.text[i++] = advance();
        }
        tok.text[i] = '\0';
        return tok;
    }

    // Handle numbers
    if (is_digit(ch)) {
        tok.type = TOK_NUMBER;

        // Add characters of the number to the token text
        int i = 0;
        while (is_digit(peek()) && i < 63) {
            tok.text[i++] = advance();
        }
        tok.text[i] = '\0';
        return tok;
    }

    // Handle strings
    if (ch == '"') {
        tok.type = TOK_STRING;
        advance();  // Skip opening quote

        // Add characters of the string to the token text
        int i = 0;
        while (peek() != '"' && peek() != '\n' && !at_end() && i < 63) {
            tok.text[i++] = advance();
        }
        tok.text[i] = '\0';

        if (peek() == '"') {
            advance();  // Skip closing quote
        } else {
            printf("Error at line %d, col %d: unterminated string\n",
                   tok.line, tok.col);
            exit(1);  // Exit the program with an error
        }
        return tok;
    }

    // Handle unknown character
    printf("Error at line %d, col %d: unexpected '%c'\n", line, col, ch);
    exit(1);  // Exit the program with an error
}
