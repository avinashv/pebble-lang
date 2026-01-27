#ifndef PEBBLE_INTERPRETER_H
#define PEBBLE_INTERPRETER_H

#include "runtime.h"
#include "opcode.h"
#include "parser.h"

// Convert an operand to a runtime value
Value resolve(RuntimeState *runtime, Operand *op, int line, int col);

// Print a value
void print_value(Value *v, RuntimeState *runtime);

// Execute the program
void run(RuntimeState *runtime, ParserState *parser);

#endif // PEBBLE_INTERPRETER_H
