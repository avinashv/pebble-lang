#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "interpreter.h"
#include "limits.h"

// Function pointer types for table-driven dispatch
typedef double (*ArithmeticOp)(double, double);
typedef int (*CompareOp)(double, double);

// Arithmetic operation functions
static double op_add(double a, double b) { return a + b; }
static double op_sub(double a, double b) { return a - b; }
static double op_mul(double a, double b) { return a * b; }
static double op_div(double a, double b) { return a / b; }
static double op_mod(double a, double b) { return fmod(a, b); }

// Comparison operation functions
static int cmp_eq(double a, double b)    { return a == b; }
static int cmp_noteq(double a, double b) { return a != b; }
static int cmp_lt(double a, double b)    { return a < b; }
static int cmp_lteq(double a, double b)  { return a <= b; }
static int cmp_gt(double a, double b)    { return a > b; }
static int cmp_gteq(double a, double b)  { return a >= b; }

// Lookup tables for arithmetic and comparison operations
static const struct {
    ArithmeticOp fn;
    const char *name;
} arith_ops[] = {
    [OP_ADD] = { op_add, "add" },
    [OP_SUB] = { op_sub, "sub" },
    [OP_MUL] = { op_mul, "mul" },
    [OP_DIV] = { op_div, "div" },
    [OP_MOD] = { op_mod, "mod" },
};

static const CompareOp compare_ops[] = {
    [OP_EQ]    = cmp_eq,
    [OP_NOTEQ] = cmp_noteq,
    [OP_LT]    = cmp_lt,
    [OP_LTEQ]  = cmp_lteq,
    [OP_GT]    = cmp_gt,
    [OP_GTEQ]  = cmp_gteq,
};

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
void print_value(Value *v, RuntimeState *runtime) {
    if (v->type == VAL_NUMBER) {
        // Check if it's a whole number
        if (v->number == (long)v->number) {
            printf("%ld\n", (long)v->number);
        } else {
            printf("%g\n", v->number);
        }
    } else if (v->type == VAL_STRING) {
        // Print string as is
        printf("%s\n", v->string);
    } else if (v->type == VAL_ARRAY) {
        // Print array as [elem1, elem2, ...]
        Array *arr = get_array(runtime, v->array_index);
        printf("[");
        for (int i = 0; i < arr->length; i++) {
            if (i > 0) printf(", ");
            if (arr->elements[i] == (long)arr->elements[i]) {
                printf("%ld", (long)arr->elements[i]);
            } else {
                printf("%g", arr->elements[i]);
            }
        }
        printf("]\n");
    }
}

// Execute an arithmetic operation (add, sub, mul, div, mod).
static void exec_arithmetic(RuntimeState *runtime, Instruction *inst, ArithmeticOp op, const char *op_name) {
    // Resolve the first two operands to values
    Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
    Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
    
    // Check if the operands are numbers
    if (a.type != VAL_NUMBER || b.type != VAL_NUMBER) {
        printf("Error at line %d: %s requires numbers\n", inst->line, op_name);
        exit(1);
    }
    
    // Create a new result value and set the third operand to the result
    Value result;
    result.type = VAL_NUMBER;
    result.number = op(a.number, b.number);
    set_variable(runtime, inst->operands[2].name, result);
}

// Execute a comparison operation (eq, noteq, lt, lteq, gt, gteq).
static void exec_compare(RuntimeState *runtime, Instruction *inst, CompareOp op) {
    // Resolve the first two operands to values
    Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
    Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);

    // Execute the comparison operation and set the result to the runtime state
    runtime->comparison_result = op(a.number, b.number);
}

// Resolve a jump target label to an instruction address.
// Returns the address, or exits with an error if the label is not found.
static int resolve_jump(ParserState *parser, Instruction *inst) {
    const char *target = inst->operands[0].name;
    int addr = find_label(parser, target);
    if (addr < 0) {
        printf("Error at line %d: undefined label '%s'\n", inst->line, target);
        exit(1);
    }
    return addr;
}

// Main interpreter loop
void run(RuntimeState *runtime, ParserState *parser) {
    int pc = 0;  // Program counter
    
    while (pc < parser->instruction_count) {
        Instruction *inst = &parser->instructions[pc];
        
        switch (inst->opcode) {
            
            // Arithmetic operations
            case OP_ADD:
            case OP_SUB:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD:
                exec_arithmetic(runtime, inst, arith_ops[inst->opcode].fn, arith_ops[inst->opcode].name);
                break;
            
            // Comparison operations
            case OP_EQ:
            case OP_NOTEQ:
            case OP_LT:
            case OP_LTEQ:
            case OP_GT:
            case OP_GTEQ:
                exec_compare(runtime, inst, compare_ops[inst->opcode]);
                break;
            
            // Variable and I/O operations
            case OP_SET: {
                // Resolve the second operand to a value
                Value val = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                set_variable(runtime, inst->operands[0].name, val);
                break;
            }

            case OP_COPY: {
                // Get the value of the first operand
                Value val = get_variable(runtime, inst->operands[0].name, inst->line, inst->col);
                
                // Deep copy arrays
                if (val.type == VAL_ARRAY) {
                    val.array_index = copy_array(runtime, val.array_index, inst->line);
                }
                
                set_variable(runtime, inst->operands[1].name, val);
                break;
            }

            case OP_PRINT: {
                // Resolve the operand to a value and print it
                Value val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                print_value(&val, runtime);
                break;
            }

            case OP_READ: {
                // Read user input into a buffer and convert it to a number or string
                char input_buffer[MAX_STRING_LEN];

                // Print the prompt
                printf("%s", inst->operands[0].string);

                // Read user input into the buffer
                fgets(input_buffer, sizeof(input_buffer), stdin);
                input_buffer[strcspn(input_buffer, "\n")] = 0;

                // Convert the input to a number or string
                char *endptr;
                double num_val = strtod(input_buffer, &endptr);

                // Create a new result value and set it to the number or string
                Value result;

                // If the input is a number, set the result to the number
                if (*endptr == '\0' && input_buffer[0] != '\0') {
                    result.type = VAL_NUMBER;
                    result.number = num_val;
                } else {
                    // If the input is a string, set the result to the string
                    result.type = VAL_STRING;
                    strcpy(result.string, input_buffer);
                }

                // Set the second operand to the result
                set_variable(runtime, inst->operands[1].name, result);
                break;
            }
            
            // String operations
            case OP_CONCAT: {
                // Resolve the first two operands to values
                Value a = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value b = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                
                // Check if the operands are strings
                if (a.type != VAL_STRING || b.type != VAL_STRING) {
                    printf("Error at line %d: concat requires strings\n", inst->line);
                    exit(1);
                }

                // Create a new result value and set it to the concatenated string
                Value result;
                result.type = VAL_STRING;
                strcpy(result.string, a.string);
                strcat(result.string, b.string);
                set_variable(runtime, inst->operands[2].name, result);
                break;
            }
            
            // Control flow
            case OP_HALT:
                return;
            
            case OP_LABEL:
                // Labels are no-ops at runtime
                break;
            
            case OP_JUMP:
                pc = resolve_jump(parser, inst);
                continue;  // Skip the pc++ at the end
            
            case OP_JUMP_IF:
            case OP_JUMP_NOT: {
                // Determine whether the jump should happen based on the comparison result
                int should_jump = (inst->opcode == OP_JUMP_IF) ? runtime->comparison_result : !runtime->comparison_result;

                // If the jump should happen, resolve the jump target and skip the pc++ at the end
                if (should_jump) {
                    pc = resolve_jump(parser, inst);
                    continue;  // Skip the pc++ at the end
                }

                // If the jump should not happen, break
                break;
            }

            case OP_CALL: {
                // Check for call stack overflow
                if (runtime->call_stack_top >= MAX_CALL_DEPTH) {
                    printf("Error at line %d: call stack overflow\n", inst->line);
                    exit(1);
                }

                // Push return address and jump to subroutine
                runtime->call_stack[runtime->call_stack_top++] = pc + 1;
                pc = resolve_jump(parser, inst);
                continue;
            }

            case OP_RET: {
                // Check for call stack underflow
                if (runtime->call_stack_top <= 0) {
                    printf("Error at line %d: ret without matching call\n", inst->line);
                    exit(1);
                }

                // Pop return address and jump back
                pc = runtime->call_stack[--runtime->call_stack_top];
                continue;
            }

            // Array and length operations
            case OP_LEN: {
                // Get length of string or array
                Value val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                
                Value result;
                result.type = VAL_NUMBER;
                
                if (val.type == VAL_STRING) {
                    result.number = strlen(val.string);
                } else if (val.type == VAL_ARRAY) {
                    Array *arr = get_array(runtime, val.array_index);
                    result.number = arr->length;
                } else {
                    printf("Error at line %d: len requires a string or array\n", inst->line);
                    exit(1);
                }
                
                set_variable(runtime, inst->operands[1].name, result);
                break;
            }

            case OP_ARRAY: {
                // Create a new array: array <name> <size>
                Value size_val = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                
                if (size_val.type != VAL_NUMBER) {
                    printf("Error at line %d: array size must be a number\n", inst->line);
                    exit(1);
                }
                
                int capacity = (int)size_val.number;
                int arr_index = alloc_array(runtime, capacity, inst->line);
                
                Value arr_val;
                arr_val.type = VAL_ARRAY;
                arr_val.array_index = arr_index;
                
                set_variable(runtime, inst->operands[0].name, arr_val);
                break;
            }

            case OP_SETA: {
                // Set array element: seta <arr> <idx> <val>
                Value arr_val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value idx_val = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                Value val = resolve(runtime, &inst->operands[2], inst->line, inst->col);
                
                if (arr_val.type != VAL_ARRAY) {
                    printf("Error at line %d: seta requires an array\n", inst->line);
                    exit(1);
                }
                if (idx_val.type != VAL_NUMBER) {
                    printf("Error at line %d: array index must be a number\n", inst->line);
                    exit(1);
                }
                if (val.type != VAL_NUMBER) {
                    printf("Error at line %d: array elements must be numbers\n", inst->line);
                    exit(1);
                }
                
                Array *arr = get_array(runtime, arr_val.array_index);
                int idx = (int)idx_val.number;
                
                if (idx < 0 || idx >= arr->length) {
                    printf("Error at line %d: array index %d out of bounds (length %d)\n", 
                           inst->line, idx, arr->length);
                    exit(1);
                }
                
                arr->elements[idx] = val.number;
                break;
            }

            case OP_GETA: {
                // Get array element: geta <arr> <idx> <dest>
                Value arr_val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value idx_val = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                
                if (arr_val.type != VAL_ARRAY) {
                    printf("Error at line %d: geta requires an array\n", inst->line);
                    exit(1);
                }
                if (idx_val.type != VAL_NUMBER) {
                    printf("Error at line %d: array index must be a number\n", inst->line);
                    exit(1);
                }
                
                Array *arr = get_array(runtime, arr_val.array_index);
                int idx = (int)idx_val.number;
                
                if (idx < 0 || idx >= arr->length) {
                    printf("Error at line %d: array index %d out of bounds (length %d)\n", 
                           inst->line, idx, arr->length);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = arr->elements[idx];
                
                set_variable(runtime, inst->operands[2].name, result);
                break;
            }

            case OP_PUSH: {
                // Push element to array: push <arr> <val>
                Value arr_val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                Value val = resolve(runtime, &inst->operands[1], inst->line, inst->col);
                
                if (arr_val.type != VAL_ARRAY) {
                    printf("Error at line %d: push requires an array\n", inst->line);
                    exit(1);
                }
                if (val.type != VAL_NUMBER) {
                    printf("Error at line %d: array elements must be numbers\n", inst->line);
                    exit(1);
                }
                
                Array *arr = get_array(runtime, arr_val.array_index);
                
                if (arr->length >= arr->capacity) {
                    printf("Error at line %d: array capacity exceeded\n", inst->line);
                    exit(1);
                }
                
                arr->elements[arr->length++] = val.number;
                break;
            }

            case OP_POP: {
                // Pop element from array: pop <arr> <dest>
                Value arr_val = resolve(runtime, &inst->operands[0], inst->line, inst->col);
                
                if (arr_val.type != VAL_ARRAY) {
                    printf("Error at line %d: pop requires an array\n", inst->line);
                    exit(1);
                }
                
                Array *arr = get_array(runtime, arr_val.array_index);
                
                if (arr->length <= 0) {
                    printf("Error at line %d: cannot pop from empty array\n", inst->line);
                    exit(1);
                }
                
                Value result;
                result.type = VAL_NUMBER;
                result.number = arr->elements[--arr->length];
                
                set_variable(runtime, inst->operands[1].name, result);
                break;
            }

            default:
                break;
        }
        
        // Increment the program counter
        pc++;
    }
}
