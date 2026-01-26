#ifndef PEBBLE_PARSER_H
#define PEBBLE_PARSER_H

#include "opcode.h"
#include "token.h"
#include "lexer.h"

// Maximum number of instructions
#define MAX_INSTRUCTIONS 1024

// Parser state
typedef struct ParserState {
    Token current_token;                    // The current token being processed
    Instruction instructions[MAX_INSTRUCTIONS]; // The parsed instructions
    int instruction_count;                  // The number of instructions in the program
} ParserState;

// Initialize parser state
void parser_init(ParserState *state);

// Parse the entire program (requires lexer to be initialized first)
void parse(ParserState *parser, LexerState *lexer);

#endif // PEBBLE_PARSER_H
