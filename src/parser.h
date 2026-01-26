#ifndef PEBBLE_PARSER_H
#define PEBBLE_PARSER_H

#include "opcode.h"

// Maximum number of instructions
#define MAX_INSTRUCTIONS 1024

// Parser state - exposed for main to access parsed results
extern Instruction instructions[MAX_INSTRUCTIONS];
extern int instruction_count;

// Parse the entire program (requires lexer to be initialized first)
void parse(void);

#endif // PEBBLE_PARSER_H
