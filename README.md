# Pebble

Pebble is a simple, assembly-like programming language with a clean, readable syntax. It features a small instruction set that makes it easy to learn while still being capable of solving real problems.

## Features

- Simple, line-based syntax
- Dynamic typing with numbers and strings
- Labels and conditional jumps for control flow
- Arithmetic and comparison operations
- String concatenation
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

Pebble supports two data types:

- **Numbers**: Integers and floating-point values (e.g., `42`, `3.14`)
- **Strings**: Text enclosed in double quotes (e.g., `"Hello, World!"`)

### Variables

Variables are created automatically when first assigned. Names must start with a letter or underscore and can contain letters, digits, and underscores.

### Instructions

#### Variables and I/O

| Instruction | Syntax | Description |
|-------------|--------|-------------|
| `set` | `set <var> <value>` | Assign a value to a variable |
| `copy` | `copy <src> <dest>` | Copy one variable to another |
| `print` | `print <value>` | Print a value to stdout |
| `read` | `read <prompt> <var>` | Read user input into a variable |

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

## Project Structure

```
pebble-lang/
├── src/
│   ├── pebble.c       # Main entry point
│   ├── lexer.c/h      # Tokenizer
│   ├── parser.c/h     # Parser
│   ├── opcode.c/h     # Instruction definitions
│   ├── interpreter.c/h # Execution engine
│   ├── runtime.c/h    # Runtime state and variables
│   ├── token.c/h      # Token types
│   └── utils.c/h      # Utility functions
├── examples/          # Example programs
├── Makefile
├── LICENSE
└── README.md
```

## License

Pebble is released under the MIT License. See [LICENSE](LICENSE) for details.
