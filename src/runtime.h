#ifndef PEBBLE_RUNTIME_H
#define PEBBLE_RUNTIME_H

// Runtime value types
typedef enum {
    VAL_INT,
    VAL_STRING,
} ValueType;

// Runtime value structure
typedef struct {
    ValueType type;
    long integer;     // For VAL_INT
    char string[256]; // For VAL_STRING
} Value;

// Variable type
typedef struct {
    char name[64];
    Value value;
    int used; // Whether the variable has been used
} Variable;

// Maximum number of variables
#define MAX_VARIABLES 1024

// Runtime state (things that change during execution)
typedef struct {
    Variable variables[MAX_VARIABLES];
    int variable_count;

    int comparison_result; // Result of the last comparison operation
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
