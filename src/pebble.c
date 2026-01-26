#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "opcode.h"

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

// Variables storage
#define MAX_VARIABLES 1024
Variable variables[MAX_VARIABLES];
int variable_count = 0; // The number of variables in the program

// Find a variable by name
Variable *find_variable(const char *name) {
    // Go through all variables
    for (int i = 0; i < variable_count; i++) {
        // If the variable is used and the name matches, return the variable
        if (variables[i].used && strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }

    // If no variable is found, return NULL
    return NULL;
}

// Set a variable value
void set_variable(const char *name, Value value) {
    Variable *var = find_variable(name);
    if (var != NULL) {
        var->value = value;
        return;
    }

    // If no variable is found, create a new one
    if (variable_count >= MAX_VARIABLES) {
        printf("Error: maximum number of variables reached\n");
        exit(1);
    }

    // Create a new variable
    Variable *new_var = &variables[variable_count++];
    strcpy(new_var->name, name);
    new_var->value = value;
    new_var->used = 1;
}

// Get a variable value
Value get_variable(const char *name) {
    Variable *var = find_variable(name);

    if (var == NULL) {
        printf("Error at line %d, col %d: undefined variable '%s'\n", line, col, name);
        exit(1);
    }

    return var->value;
}

int main(int argc, char **argv) {
    // Check command-line arguments
    if (argc < 2) {
        printf("Pebble Interpreter v0.1\n");
        printf("Usage: pebble <filename>\n");
        return 1;
    }

    // Read the file into memory
    char *source = read_file(argv[1]);
    lexer_init(source);
    parse();

    // Print all parsed instructions
    printf("Parsed %d instructions:\n\n", instruction_count);

    // Go through all instructions
    for (int i = 0; i < instruction_count; i++) {
        // Get the instruction
        Instruction *instr = &instructions[i];

        printf("%3d: %s", i, opcode_name(instr->opcode));

        // Go through all operands
        for (int j = 0; j < instr->operand_count; j++) {
            Operand *op = &instr->operands[j];

            // Print the operand
            switch (op->type) {
                case OPER_IDENT:
                    printf(" %s", op->name);
                    break;
                case OPER_INT:
                    printf(" %ld", op->value);
                    break;
                case OPER_STRING:
                    printf(" \"%s\"", op->string);
                    break;
            }
        }

        // Print a newline to close the line
        printf("\n");
    }

    free(source);

    return 0;
}
