#ifndef PEBBLE_LIMITS_H
#define PEBBLE_LIMITS_H

// Pebble Language Limits

// Maximum length for identifiers (variable names, label names, etc.)
// Actual usable length is MAX_IDENT_LEN - 1 due to null terminator
#define MAX_IDENT_LEN 64

// Maximum length for string literals and string values
// Actual usable length is MAX_STRING_LEN - 1 due to null terminator
#define MAX_STRING_LEN 256

// Maximum number of operands per instruction
#define MAX_OPERANDS 3

// Maximum number of instructions in a program
#define MAX_INSTRUCTIONS 1024

// Maximum number of labels in a program
#define MAX_LABELS 128

// Maximum number of variables at runtime
#define MAX_VARIABLES 1024

// Maximum call stack depth (for nested subroutine calls)
#define MAX_CALL_DEPTH 128

// Maximum number of arrays at runtime
#define MAX_ARRAYS 64

// Maximum number of elements per array
#define MAX_ARRAY_LEN 128

#endif // PEBBLE_LIMITS_H
