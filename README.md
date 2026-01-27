# Pebble

Pebble is a simple, assembly-like programming language with a clean, readable syntax. It features a small instruction set that makes it easy to learn while still being capable of solving real problems.

## Features

- Simple, line-based syntax
- Dynamic typing with numbers, strings, and arrays
- Labels and conditional jumps for control flow
- Subroutines with `call` and `ret`
- Arithmetic and comparison operations
- String concatenation
- Arrays with push/pop operations
- User input/output
- Comments with `#`

## Building

Pebble is written in C99 and has no external dependencies.

```bash
make
```

This produces the `pebble` executable.

## Usage

```bash
./pebble <filename.pbl>
```

## Language Reference

### Data Types

Pebble supports three data types:

- **Numbers**: Integers and floating-point values (e.g., `42`, `3.14`)
- **Strings**: Text enclosed in double quotes (e.g., `"Hello, World!"`)
- **Arrays**: Ordered collections of numbers (created with `array`)

### Variables

Variables are created automatically when first assigned. Names must start with a letter or underscore and can contain letters, digits, and underscores.

### Instructions

#### Variables and I/O

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `set` | `set <var> <value>` | Assign a value to a variable |
| `copy` | `copy <src> <dest>` | Copy one variable to another (deep copy for arrays) |
| `print` | `print <value>` | Print a value to stdout |
| `read` | `read <prompt> <var>` | Read user input into a variable |
| `len` | `len <var> <dest>` | Get length of a string or array |

#### Arithmetic

All arithmetic operations require numeric operands and store the result in a destination variable.

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `add` | `add <a> <b> <dest>` | dest = a + b |
| `sub` | `sub <a> <b> <dest>` | dest = a - b |
| `mul` | `mul <a> <b> <dest>` | dest = a * b |
| `div` | `div <a> <b> <dest>` | dest = a / b |
| `mod` | `mod <a> <b> <dest>` | dest = a mod b |

#### String Operations

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `concat` | `concat <a> <b> <dest>` | Concatenate two strings |

#### Array Operations

Arrays hold numeric values only. When created, arrays are initialized with zeros.

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `array` | `array <name> <size>` | Create an array with given size |
| `seta` | `seta <arr> <idx> <val>` | Set array element: arr[idx] = val |
| `geta` | `geta <arr> <idx> <dest>` | Get array element: dest = arr[idx] |
| `push` | `push <arr> <val>` | Append value to end of array |
| `pop` | `pop <arr> <dest>` | Remove and return last element |

Note: `push` fails if the array is at capacity, `pop` fails if the array is empty. Use `len` to get the current array length.

#### Comparisons

Comparison instructions set an internal flag used by conditional jumps.

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `eq` | `eq <a> <b>` | Test if a == b |
| `noteq` | `noteq <a> <b>` | Test if a != b |
| `lt` | `lt <a> <b>` | Test if a < b |
| `lteq` | `lteq <a> <b>` | Test if a <= b |
| `gt` | `gt <a> <b>` | Test if a > b |
| `gteq` | `gteq <a> <b>` | Test if a >= b |

#### Control Flow

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `label` | `label <name>` | Define a jump target |
| `jump` | `jump <label>` | Unconditional jump |
| `jumpif` | `jumpif <label>` | Jump if last comparison was true |
| `jumpnot` | `jumpnot <label>` | Jump if last comparison was false |
| `halt` | `halt` | Stop program execution |

#### Subroutines

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `call` | `call <label>` | Call a subroutine (pushes return address) |
| `ret` | `ret` | Return from subroutine (pops return address) |

Subroutines use labels as entry points. Variables are global, so subroutines can access and modify any variable.

### Comments

Lines starting with `#` are comments:

```
# This is a comment
set x 10  # This is also a comment
```

## Examples

### Hello World

```
# Hello, World!
set msg "Hello, World!"
print msg
halt
```

### Countdown

```
# Countdown from 5 to 1
set n 5

label loop
    print n
    sub n 1 n
    gt n 0
    jumpif loop

print "Liftoff!"
halt
```

### Factorial

```
# Calculate factorial
read "Enter a number: " n
copy n result

label loop
    sub n 1 n
    gt n 1
    jumpnot done
    mul result n result
    jump loop

label done
    print result
    halt
```

### Fibonacci Sequence

```
# Print Fibonacci numbers
read "How many Fibonacci numbers? " count

set a 1
set b 1
set i 0

label loop
    lt i count
    jumpnot done
    print a
    add a b next
    copy b a
    copy next b
    add i 1 i
    jump loop

label done
    halt
```

### Interactive Greeting

```
# Greet the user
read "What is your name? " name
set greeting "Hello, "
concat greeting name greeting
concat greeting "!" greeting
print greeting
halt
```

### Array Sum

```
# Sum all elements in an array
array nums 5
seta nums 0 10
seta nums 1 20
seta nums 2 30
seta nums 3 40
seta nums 4 50

set sum 0
set i 0
len nums size

label loop
    lt i size
    jumpnot done
    geta nums i val
    add sum val sum
    add i 1 i
    jump loop

label done
    print sum
    halt
```

### Subroutine Example

```
# Using subroutines to organize code
set x 5
call double
print x
halt

label double
    mul x 2 x
    ret
```

## Limits

Pebble has the following built-in limits:

| Limit | Value | Description |
|-------|-------|-------------|
| Identifier length | 63 characters | Maximum length for variable and label names |
| String length | 255 characters | Maximum length for string literals and values |
| Instructions | 1024 | Maximum number of instructions per program |
| Variables | 1024 | Maximum number of variables at runtime |
| Labels | 128 | Maximum number of labels per program |
| Call stack depth | 128 | Maximum nested subroutine calls |
| Arrays | 64 | Maximum number of arrays at runtime |
| Array elements | 128 | Maximum elements per array |

## Project Structure

```
pebble-lang/
├── src/
│   ├── pebble.c         # Main entry point
│   ├── lexer.c/h        # Tokenizer
│   ├── parser.c/h       # Parser
│   ├── opcode.c/h       # Instruction definitions
│   ├── interpreter.c/h  # Execution engine
│   ├── runtime.c/h      # Runtime state, variables, and arrays
│   ├── token.c/h        # Token types
│   ├── limits.h         # Compile-time limits
│   └── utils.c/h        # Utility functions
├── examples/            # Example programs
├── Makefile
├── LICENSE
└── README.md
```

## License

Pebble is released under the MIT License. See [LICENSE](LICENSE) for details.
