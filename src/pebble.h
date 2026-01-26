#ifndef PEBBLE_H
#define PEBBLE_H

#include "lexer.h"
#include "parser.h"
#include "runtime.h"

// Top-level interpreter state, composing all sub-states
typedef struct {
    LexerState lexer;
    ParserState parser;
    RuntimeState runtime;
} PebbleState;

// Initialize all interpreter state
void pebble_init(PebbleState *state, const char *source);

#endif // PEBBLE_H
