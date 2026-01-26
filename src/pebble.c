#include <stdio.h>
#include <stdlib.h>
#include "pebble.h"
#include "utils.h"
#include "interpreter.h"

// Initialize all interpreter state
void pebble_init(PebbleState *state) {
    parser_init(&state->parser);
    runtime_init(&state->runtime);
    // Note: lexer_init is called separately with the source code
}

int main(int argc, char **argv) {
    // Check command-line arguments
    if (argc < 2) {
        printf("Pebble Interpreter v0.1\n");
        printf("Usage: pebble <filename>\n");
        return 1;
    }

    // Initialize interpreter state
    PebbleState state;
    pebble_init(&state);

    // Read the file and initialize lexer
    char *source = read_file(argv[1]);
    lexer_init(&state.lexer, source);

    // Parse the program
    parse(&state.parser, &state.lexer);

    // Clear the memory of the source code
    free(source);
    
    // Run the program
    run(&state.runtime, &state.parser);

    return 0;
}
