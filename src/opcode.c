#include <stddef.h>
#include <string.h>
#include "opcode.h"

// OpCode table
OpCodeInfo opcode_table[] = {
    { "set", OP_SET, 2 },
    { "copy", OP_COPY, 2 },
    { "print", OP_PRINT, 1 },
    { "read", OP_READ, 2 },
    { "add", OP_ADD, 3 },
    { "sub", OP_SUB, 3 },
    { "mul", OP_MUL, 3 },
    { "div", OP_DIV, 3 },
    { "mod", OP_MOD, 3 },
    { "halt", OP_HALT, 0 },
    { "label", OP_LABEL, 1 },
    { "jump", OP_JUMP, 1 },
    { "jumpif", OP_JUMP_IF, 1 },
    { "jumpnot", OP_JUMP_NOT, 1 },
    { "eq", OP_EQ, 2 },
    { "noteq", OP_NOTEQ, 2 },
    { "lt", OP_LT, 2 },
    { "lteq", OP_LTEQ, 2 },
    { "gt", OP_GT, 2 },
    { "gteq", OP_GTEQ, 2 },
    { "concat", OP_CONCAT, 3 },
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
