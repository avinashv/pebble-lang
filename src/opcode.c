#include <stddef.h>
#include <string.h>
#include "opcode.h"

// OpCode table
OpCodeInfo opcode_table[] = {
    { "set", OP_SET, 2 },
    { "print", OP_PRINT, 1 },
    { "add", OP_ADD, 3 },
    { "sub", OP_SUB, 3 },
    { "halt", OP_HALT, 0 },
    { NULL, 0, 0 } // End of table marker
};

// Look up opcode by name
OpCodeInfo *find_opcode(const char *name) {
    for (int i = 0; opcode_table[i].name != NULL; i++) {
        if (strcmp(opcode_table[i].name, name) == 0) {
            return &opcode_table[i];
        }
    }
    return NULL;
}

// Get OpCode name for printing
const char *opcode_name(OpCode opcode) {
    for (int i = 0; opcode_table[i].name != NULL; i++) {
        if (opcode_table[i].opcode == opcode) {
            return opcode_table[i].name;
        }
    }
    return "UNKNOWN";
}
