# Compiler settings
CC = cc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g

# The target executable
TARGET = pebble

# Source files
SRCS = pebble.c
HDRS =

# Default target: build the program
all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

# Remove built files
clean:
	rm -f $(TARGET)

# Phony targets aren't files
.PHONY: all clean