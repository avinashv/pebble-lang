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

#endif // PEBBLE_LIMITS_H
