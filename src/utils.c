#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// Read the file into memory
char *read_file(const char *path) {
    // Open the file
    FILE *file = fopen(path, "rb");
    if (!file) {
        printf("Cannot open file: %s\n", path);
        exit(1);
    }

    // Find file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the file and read
    char *buffer = malloc(size + 1);
    if (!buffer) {
        printf("Cannot allocate memory for file: %s\n", path);
        exit(1);
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    // Close the file
    fclose(file);

    // Return the buffer
    return buffer;
}
