#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"

/*
 * Convert an operand to a runtime value.
 */
Value resolve(RuntimeState *runtime, Operand *op, int line, int col) {
    Value v;
    
    switch (op->type) {
        case OPER_INT:
            v.type = VAL_INT;
            v.integer = op->value;
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

/*
 * Print a value.
 */
void print_value(Value *v) {
    if (v->type == VAL_INT) {
        printf("%ld\n", v->integer);
    } else {
        printf("%s\n", v->string);
    }
}

/*
 * Execute the program.
 */
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
            
            case OP_PRINT: {
                Value val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                print_value(&val);
                pc++;
                break;
            }
            
            case OP_ADD: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_INT || b.type != VAL_INT) {
                    printf("Error at line %d: add requires integers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_INT;
                result.integer = a.integer + b.integer;
                set_variable(runtime, dest, result);
                pc++;
                break;
            }
            
            case OP_SUB: {
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                const char *dest = inst->operands[2].name;
                
                if (a.type != VAL_INT || b.type != VAL_INT) {
                    printf("Error at line %d: sub requires integers\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_INT;
                result.integer = a.integer - b.integer;
                set_variable(runtime, dest, result);
                pc++;
                break;
            }
            
            case OP_HALT:
                return;  /* Stop execution */
            
            default:
                pc++;
                break;
        }
    }
}
