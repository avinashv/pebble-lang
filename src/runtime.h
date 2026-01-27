#ifndef PEBBLE_RUNTIME_H
#define PEBBLE_RUNTIME_H

#include <stdbool.h>
#include "limits.h"

// Runtime value types
typedef enum {
    VAL_NUMBER,
    VAL_STRING,
} ValueType;

// Runtime value structure
typedef struct {
    ValueType type;
    double number;               // For VAL_NUMBER
    char string[MAX_STRING_LEN]; // For VAL_STRING
} Value;

// Variable type
typedef struct {
    char name[MAX_IDENT_LEN];
    Value value;
    bool used; // Whether the variable has been used
} Variable;

// Runtime state (things that change during execution)
typedef struct {
    Variable variables[MAX_VARIABLES];
    int variable_count;

    bool comparison_result; // Result of the last comparison operation

    int call_stack[MAX_CALL_DEPTH]; // Stack of return addresses
    int call_stack_top;             // Index of next free slot (0 = empty)
} RuntimeState;

// Initialize runtime state
void runtime_init(RuntimeState *state);

// Find a variable by name (returns NULL if not found)
Variable *find_variable(RuntimeState *state, const char *name);

// Set a variable value (creates if doesn't exist)
void set_variable(RuntimeState *state, const char *name, Value value);

// Get a variable value (exits with error if not found)
Value get_variable(RuntimeState *state, const char *name, int line, int col);

#endif // PEBBLE_RUNTIME_H
