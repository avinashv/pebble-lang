#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Initialize parser state
void parser_init(ParserState *state) {
    state->instruction_count = 0;
    state->label_count = 0;
}

// Record a label's position in the instruction array
void record_label(ParserState *state, const char *name, int address) {
    if (state->label_count >= MAX_LABELS) {
        printf("Error: maximum number of labels reached\n");
        exit(1);
    }

    // Create a new label
    strcpy(state->labels[state->label_count].name, name);
    state->labels[state->label_count].address = address;
    state->label_count++;
}

// Find a label by name
int find_label(ParserState *state, const char *name) {
    // Go through all labels
    for (int i = 0; i < state->label_count; i++) {
        // If the label name matches, return the address
        if (strcmp(state->labels[i].name, name) == 0) {
            return state->labels[i].address;
        }
    }
    
    // If no label is found, return -1
    return -1;
}

// Move to the next token
static void parser_advance(ParserState *parser, LexerState *lexer) {
    parser->current_token = next_token(lexer);
}

// Parse one operand
static Operand parse_operand(ParserState *parser, LexerState *lexer) {
    Operand op;

    // Parse the operand based on the token type
    switch (parser->current_token.type) {
        case TOK_WORD:
            op.type = OPER_IDENT;
            strcpy(op.name, parser->current_token.text);
            break;
        case TOK_NUMBER:
            op.type = OPER_NUMBER;
            op.value = strtod(parser->current_token.text, NULL); // Convert string to double
            break;
        case TOK_STRING:
            op.type = OPER_STRING;
            strcpy(op.string, parser->current_token.text);
            break;
        default:
            printf("Error at line %d, col %d: expected operand, got %s\n", 
                   parser->current_token.line, parser->current_token.col, 
                   token_type_name(parser->current_token.type));
            exit(1);
    }

    // Advance to the next token
    parser_advance(parser, lexer);

    // Return the operand
    return op;
}

// Parse one instruction
static void parse_instruction(ParserState *parser, LexerState *lexer) {
    // First token should be the instruction name
    if (parser->current_token.type != TOK_WORD) {
        printf("Error at line %d, col %d: expected instruction, got %s\n", 
               parser->current_token.line, parser->current_token.col, 
               token_type_name(parser->current_token.type));
        exit(1);
    }

    // Look up the opcode
    OpCodeInfo *opcode_info = find_opcode(parser->current_token.text);
    if (!opcode_info) {
        printf("Error at line %d, col %d: unknown instruction '%s'\n", 
               parser->current_token.line, parser->current_token.col, 
               parser->current_token.text);
        exit(1);
    }

    // Create the instruction
    Instruction instr;
    instr.opcode = opcode_info->opcode;
    instr.line = parser->current_token.line;
    instr.col = parser->current_token.col;
    instr.operand_count = opcode_info->operand_count;

    parser_advance(parser, lexer);

    // Parse the operands
    for (int i = 0; i < opcode_info->operand_count; i++) {
        if (parser->current_token.type == TOK_NEWLINE || parser->current_token.type == TOK_EOF) {
            printf("Error at line %d, col %d: expected operand, got %s\n", 
                   parser->current_token.line, parser->current_token.col, 
                   token_type_name(parser->current_token.type));
            exit(1);
        }

        // Parse the operand and append it to the instruction
        instr.operands[i] = parse_operand(parser, lexer);
    }
    
    // Should have reached EOL/EOF now
    if (parser->current_token.type != TOK_NEWLINE && parser->current_token.type != TOK_EOF) {
        printf("Error at line %d, col %d: unexpected token, got %s\n", 
               parser->current_token.line, parser->current_token.col, 
               token_type_name(parser->current_token.type));
    }

    // Skip newline if present
    if (parser->current_token.type == TOK_NEWLINE) {
        parser_advance(parser, lexer);
    }

    // If the instruction is a label, record it
    if (instr.opcode == OP_LABEL) {
        record_label(parser, instr.operands[0].name, parser->instruction_count);

        // Don't add label as an instruction, just record its position
        return;
    }

    // Add the instruction to the program
    parser->instructions[parser->instruction_count++] = instr;
}

// Parse the entire program
void parse(ParserState *parser, LexerState *lexer) {
    // Get the first token
    parser_advance(parser, lexer);

    while (parser->current_token.type != TOK_EOF) {
        // Skip blank lines
        while (parser->current_token.type == TOK_NEWLINE) {
            parser_advance(parser, lexer);
            continue;
        }

        // Parse an instruction
        parse_instruction(parser, lexer);
    }
}
