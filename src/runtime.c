#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime.h"

// Initialize runtime state
void runtime_init(RuntimeState *state) {
    state->variable_count = 0;
    state->comparison_result = false;
    state->call_stack_top = 0;
    
    // Initialize array pool
    for (int i = 0; i < MAX_ARRAYS; i++) {
        state->arrays[i].used = false;
    }
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
    new_var->used = true;
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

// Allocate a new array with given capacity
int alloc_array(RuntimeState *state, int capacity, int line) {
    // Check capacity is valid
    if (capacity <= 0 || capacity > MAX_ARRAY_LEN) {
        printf("Error at line %d: array capacity must be between 1 and %d\n", line, MAX_ARRAY_LEN);
        exit(1);
    }

    // Find a free slot in the array pool
    for (int i = 0; i < MAX_ARRAYS; i++) {
        if (!state->arrays[i].used) {
            state->arrays[i].used = true;
            state->arrays[i].capacity = capacity;
            state->arrays[i].length = capacity;
            
            // Initialize all elements to 0
            for (int j = 0; j < capacity; j++) {
                state->arrays[i].elements[j] = 0;
            }
            
            return i;
        }
    }

    printf("Error at line %d: maximum number of arrays reached\n", line);
    exit(1);
}

// Deep copy an array
int copy_array(RuntimeState *state, int src_index, int line) {
    Array *src = &state->arrays[src_index];
    
    // Allocate new array with same capacity
    int dest_index = alloc_array(state, src->capacity, line);
    Array *dest = &state->arrays[dest_index];
    
    // Copy length and elements
    dest->length = src->length;
    for (int i = 0; i < src->length; i++) {
        dest->elements[i] = src->elements[i];
    }
    
    return dest_index;
}

// Get array pointer
Array *get_array(RuntimeState *state, int index) {
    return &state->arrays[index];
}
