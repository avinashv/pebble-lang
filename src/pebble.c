#include <stdio.h>
#include <stdlib.h>
#include "pebble.h"
#include "utils.h"
#include "interpreter.h"

// Initialize all interpreter state
void pebble_init(PebbleState *state, const char *source) {
    lexer_init(&state->lexer, source);
    parser_init(&state->parser);
    runtime_init(&state->runtime);
}

int main(int argc, char **argv) {
    // Check command-line arguments
    if (argc < 2) {
        printf("Pebble Interpreter v0.1\n");
        printf("Usage: pebble <filename>\n");
        return 1;
    }

    // Read the file, error handling is done in read_file
    char *source = read_file(argv[1]);
    
    // Initialize interpreter state
    PebbleState state;
    pebble_init(&state, source);

    // Parse the program
    parse(&state.parser, &state.lexer);

    // Clear the memory of the source code
    free(source);
    
    // Run the program
    run(&state.runtime, &state.parser);

    return 0;
}
