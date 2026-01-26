#ifndef PEBBLE_OPCODE_H
#define PEBBLE_OPCODE_H

// Opcode type
typedef enum {
    OP_SET,      // Set <var> <value>
    OP_PRINT,    // Print <value>
    OP_ADD,      // Add <var> <value>
    OP_SUB,      // Sub <var> <value>
    OP_HALT,     // Halt the program
    OP_LABEL,    // Label <name>
    OP_JUMP,     // Jump <label>
    OP_JUMP_IF,  // Jump if <label>
    OP_JUMP_NOT, // Jump if not <label>
    OP_EQ,       // Equal <var> <value>
    OP_LT,       // Less than <var> <value>
    OP_GT,       // Greater than <var> <value>
} OpCode;

// Operand types
typedef enum {
    OPER_IDENT,  // Variable name
    OPER_INT,    // Integer literal
    OPER_STRING, // String literal
} OperandType;

// Operand structure
typedef struct {
    OperandType type; // Type of operand
    char name[64];    // For OPER_IDENT
    long value;       // For OPER_INT
    char string[256]; // For OPER_STRING
} Operand;

// Instruction structure
typedef struct {
    OpCode opcode;       // Opcode
    Operand operands[3]; // Operands (max 3)
    int operand_count;   // Number of operands
    int line;            // Line number of the instruction
    int col;             // Column number of the instruction
} Instruction;

// OpCode information
typedef struct {
    const char *name; // Name of the opcode
    OpCode opcode;    // Opcode
    int operand_count; // Number of operands
} OpCodeInfo;

// Look up opcode by name
OpCodeInfo *find_opcode(const char *name);

// Get OpCode name for printing
const char *opcode_name(OpCode opcode);

#endif // PEBBLE_OPCODE_H
