#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interpreter.h"

// Convert an operand to a runtime value.
Value resolve(RuntimeState *runtime, Operand *op, int line, int col) {
    Value v;
    
    switch (op->type) {
        case OPER_NUMBER:
            v.type = VAL_NUMBER;
            v.number = op->value;
            break;
        case OPER_STRING:
            v.type = VAL_STRING;
            strcpy(v.string, op->string);
            break;
        case OPER_IDENT:
            return get_variable(runtime, op->name, line, col);
    }
    
    return v;
}

// Print a value.
void print_value(Value *v) {
    if (v->type == VAL_NUMBER) {
        // Check if it's a whole number
        if (v->number == (long)v->number) {
            printf("%ld\n", (long)v->number);
        } else {
            printf("%g\n", v->number);
        }
    } else {
        printf("%s\n", v->string);
    }
}

// Execute the program.
void run(RuntimeState *runtime, ParserState *parser) {
    int pc = 0;  /* Program counter: which instruction to execute */
    
    while (pc < parser->instruction_count) {
        Instruction *inst = &parser->instructions[pc];
        
        switch (inst->opcode) {
            
            case OP_SET: {
                const char *name = inst->operands[0].name;
                Value val = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                set_variable(runtime, name, val);
                pc++;
                break;
            }

            case OP_COPY: {
                const char *src = inst->operands[0].name;
                const char *dest = inst->operands[1].name;
                Value val = get_variable(runtime, src, inst->line, inst->col);
                set_variable(runtime, dest, val);
                pc++;
                break;
            }

            case OP_PRINT: {
                Value val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                print_value(&val);
                pc++;
                break;
            }

            case OP_READ: {
                char input_buffer[256];

                printf("%s", inst->operands[0].string);
                fgets(input_buffer, sizeof(input_buffer), stdin);
                
                input_buffer[strcspn(input_buffer, "\n")] = 0; // Remove newline

                // Try to parse as number using strtod
                char *endptr;
                double num_val = strtod(input_buffer, &endptr);

                Value result;
                if (*endptr == '\0' && input_buffer[0] != '\0') {
                    // Entire string was a valid number
                    result.type = VAL_NUMBER;
                    result.number = num_val;
                } else {
                    result.type = VAL_STRING;
                    strcpy(result.string, input_buffer);
                }

                set_variable(runtime, inst->operands[1].name, result);
                pc++;
                break;
            }
            
            case OP_ADD: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_NUMBER || b.type != VAL_NUMBER) {
                    printf("Error at line %d: add requires numbers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = a.number + b.number;
                set_variable(runtime, dest, result);
                pc++;
                break;
            }
            
            case OP_SUB: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_NUMBER || b.type != VAL_NUMBER) {
                    printf("Error at line %d: sub requires numbers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = a.number - b.number;
                set_variable(runtime, dest, result);
                pc++;
                break;
            }

            case OP_MUL: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_NUMBER || b.type != VAL_NUMBER) {
                    printf("Error at line %d: mul requires numbers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = a.number * b.number;
                set_variable(runtime, dest, result);
                pc++;
                break;
            }

            case OP_DIV: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_NUMBER || b.type != VAL_NUMBER) {
                    printf("Error at line %d: div requires numbers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = a.number / b.number;
                set_variable(runtime, dest, result);
                pc++;
                break;
            }

            case OP_MOD: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_NUMBER || b.type != VAL_NUMBER) {
                    printf("Error at line %d: mod requires numbers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = fmod(a.number, b.number);
                set_variable(runtime, dest, result);
                pc++;
                break;
            }
            
            case OP_HALT:
                return;  // Stop execution
            
            case OP_LABEL:
                // Labels don't do anything at runtime (and shouldn't appear here)
                pc++;
                break;
            
            case OP_JUMP: {
                const char *target = inst->operands[0].name;
                int addr = find_label(parser, target);
                if (addr < 0) {
                    printf("Error at line %d: undefined label '%s'\n",
                           inst->line, target);
                    exit(1);
                }
                pc = addr;
                break;
            }
            
            case OP_JUMP_IF: {
                if (runtime->comparison_result) {
                    const char *target = inst->operands[0].name;
                    int addr = find_label(parser, target);
                    if (addr < 0) {
                        printf("Error at line %d: undefined label '%s'\n",
                               inst->line, target);
                        exit(1);
                    }
                    pc = addr;
                } else {
                    pc++;
                }
                break;
            }
            
            case OP_JUMP_NOT: {
                if (!runtime->comparison_result) {
                    const char *target = inst->operands[0].name;
                    int addr = find_label(parser, target);
                    if (addr < 0) {
                        printf("Error at line %d: undefined label '%s'\n",
                               inst->line, target);
                        exit(1);
                    }
                    pc = addr;
                } else {
                    pc++;
                }
                break;
            }
            
            case OP_EQ: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                runtime->comparison_result = (a.number == b.number);
                pc++;
                break;
            }

            case OP_NOTEQ: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                runtime->comparison_result = (a.number != b.number);
                pc++;
                break;
            }
            
            case OP_LT: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                runtime->comparison_result = (a.number < b.number);
                pc++;
                break;
            }

            case OP_LTEQ: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                runtime->comparison_result = (a.number <= b.number);
                pc++;
                break;
            }
            
            case OP_GT: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                runtime->comparison_result = (a.number > b.number);
                pc++;
                break;
            }

            case OP_GTEQ: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                runtime->comparison_result = (a.number >= b.number);
                pc++;
                break;
            }

            case OP_CONCAT: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_STRING || b.type != VAL_STRING) {
                    printf("Error at line %d: concat requires strings\n", inst->line);
                    exit(1);
                }

                Value result;
                result.type = VAL_STRING;
                strcpy(result.string, a.string);
                strcat(result.string, b.string);
                set_variable(runtime, dest, result);
                pc++;
                break;
            }

            default:
                pc++;
                break;
        }
    }
}
