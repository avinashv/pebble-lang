#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

// Initialize runtime state
void runtime_init(RuntimeState *state) {
    state->variable_count = 0;
    state->comparison_result = 0;
}

// Find a variable by name
Variable *find_variable(RuntimeState *state, const char *name) {
    // Go through all variables
    for (int i = 0; i < state->variable_count; i++) {
        // If the variable is used and the name matches, return the variable
        if (state->variables[i].used && strcmp(state->variables[i].name, name) == 0) {
            return &state->variables[i];
        }
    }

    // If no variable is found, return NULL
    return NULL;
}

// Set a variable value
void set_variable(RuntimeState *state, const char *name, Value value) {
    Variable *var = find_variable(state, name);
    if (var != NULL) {
        var->value = value;
        return;
    }

    // If no variable is found, create a new one
    if (state->variable_count >= MAX_VARIABLES) {
        printf("Error: maximum number of variables reached\n");
        exit(1);
    }

    // Create a new variable
    Variable *new_var = &state->variables[state->variable_count++];
    strcpy(new_var->name, name);
    new_var->value = value;
    new_var->used = 1;
}

// Get a variable value
Value get_variable(RuntimeState *state, const char *name, int line, int col) {
    Variable *var = find_variable(state, name);

    if (var == NULL) {
        printf("Error at line %d, col %d: undefined variable '%s'\n", line, col, name);
        exit(1);
    }

    return var->value;
}
