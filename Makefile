# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g -Isrc

# The target executable
TARGET = pebble

# Source files (automatically find all .c and .h files in src/)
SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)

# Default target: build the program
all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Build and run the test pebbleprogram
run: $(TARGET)
	./$(TARGET) test.pbl

# Remove built files
clean:
	rm -f $(TARGET)

# Phony targets aren't files
.PHONY: all clean