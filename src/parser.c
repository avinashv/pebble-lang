#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"
#include "token.h"

// Parser state
static Token current_token;                    // The current token being processed
Instruction instructions[MAX_INSTRUCTIONS];    // The parsed instructions
int instruction_count = 0;                     // The number of instructions in the program

// Move to the next token
static void parser_advance(void) {
    current_token = next_token();
}

// Parse one operand
static Operand parse_operand(void) {
    Operand op;

    // Parse the operand based on the token type
    switch (current_token.type) {
        case TOK_WORD:
            op.type = OPER_IDENT;
            strcpy(op.name, current_token.text);
            break;
        case TOK_NUMBER:
            op.type = OPER_INT;
            op.value = atol(current_token.text); // Convert string to long integer
            break;
        case TOK_STRING:
            op.type = OPER_STRING;
            strcpy(op.string, current_token.text);
            break;
        default:
            printf("Error at line %d, col %d: expected operand, got %s\n", current_token.line, current_token.col, token_type_name(current_token.type));
            exit(1);
    }

    // Advance to the next token
    parser_advance();

    // Return the operand
    return op;
}

// Parse one instruction
static void parse_instruction(void) {
    // First token should be the instruction name
    if (current_token.type != TOK_WORD) {
        printf("Error at line %d, col %d: expected instruction, got %s\n", current_token.line, current_token.col, token_type_name(current_token.type));
        exit(1);
    }

    // Look up the opcode
    OpCodeInfo *opcode_info = find_opcode(current_token.text);
    if (!opcode_info) {
        printf("Error at line %d, col %d: unknown instruction '%s'\n", current_token.line, current_token.col, current_token.text);
        exit(1);
    }

    // Create the instruction
    Instruction instr;
    instr.opcode = opcode_info->opcode;
    instr.line = current_token.line;
    instr.col = current_token.col;
    instr.operand_count = opcode_info->operand_count;

    parser_advance();

    // Parse the operands
    for (int i = 0; i < opcode_info->operand_count; i++) {
        if (current_token.type == TOK_NEWLINE || current_token.type == TOK_EOF) {
            printf("Error at line %d, col %d: expected operand, got %s\n", current_token.line, current_token.col, token_type_name(current_token.type));
            exit(1);
        }

        // Parse the operand and append it to the instruction
        instr.operands[i] = parse_operand();
    }
    
    // Should have reached EOL/EOF now
    if (current_token.type != TOK_NEWLINE && current_token.type != TOK_EOF) {
        printf("Error at line %d, col %d: unexpected token, got %s\n", current_token.line, current_token.col, token_type_name(current_token.type));
    }

    // Skip newline if present
    if (current_token.type == TOK_NEWLINE) {
        parser_advance();
    }

    // Add the instruction to the program
    instructions[instruction_count++] = instr;
}

// Parse the entire program
void parse(void) {
    // Get the first token
    parser_advance();

    while (current_token.type != TOK_EOF) {
        // Skip blank lines
        while (current_token.type == TOK_NEWLINE) {
            parser_advance();
            continue;
        }

        // Parse an instruction
        parse_instruction();
    }
}
