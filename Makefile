# Detect OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

# Compiler settings
CC = cc
CFLAGS_BASE = -Wall -Wextra -pedantic -std=c99 -Isrc
CFLAGS_RELEASE = $(CFLAGS_BASE) -O3 -flto -DNDEBUG
CFLAGS_DEBUG = $(CFLAGS_BASE) -g -O0

# Platform-specific settings
ifeq ($(DETECTED_OS),Windows)
    TARGET = pebble.exe
    LDFLAGS_RELEASE = -static
    RM = del /f
else ifeq ($(DETECTED_OS),Linux)
    TARGET = pebble
    LDFLAGS_RELEASE = -static
    RM = rm -f
else
    # macOS - no static linking available
    TARGET = pebble
    LDFLAGS_RELEASE =
    RM = rm -f
endif

# Source files (automatically find all .c and .h files in src/)
SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)

# Default target: optimized release build
all: release

release: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS_RELEASE) $(LDFLAGS_RELEASE) -o $(TARGET) $(SRCS) -lm

debug: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS_DEBUG) -o $(TARGET) $(SRCS) -lm

# Remove built files
clean:
	$(RM) $(TARGET)

# Phony targets aren't files
.PHONY: all release debug clean