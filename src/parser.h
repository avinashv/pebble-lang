#ifndef PEBBLE_PARSER_H
#define PEBBLE_PARSER_H

#include "opcode.h"
#include "token.h"
#include "lexer.h"

// Maximum number of instructions
#define MAX_INSTRUCTIONS 1024

// Maximum number of labels
#define MAX_LABELS 128

// Label type (compile-time construct, resolved during parsing)
typedef struct {
    char name[64];
    int address; // Index in the instruction array
} Label;

// Parser state
typedef struct ParserState {
    Token current_token;                        // The current token being processed
    Instruction instructions[MAX_INSTRUCTIONS]; // The parsed instructions
    int instruction_count;                      // The number of instructions in the program
    Label labels[MAX_LABELS];                   // Labels and their addresses
    int label_count;                            // The number of labels in the program
} ParserState;

// Initialize parser state
void parser_init(ParserState *state);

// Parse the entire program (requires lexer to be initialized first)
void parse(ParserState *parser, LexerState *lexer);

// Record a label's position in the instruction array
void record_label(ParserState *state, const char *name, int address);

// Find a label by name (returns -1 if not found)
int find_label(ParserState *state, const char *name);

#endif // PEBBLE_PARSER_H

