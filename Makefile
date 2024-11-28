# Makefile for Arduino LED Control Project

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -Werror -pedantic -std=c11
CPPFLAGS := -I./include
LDFLAGS :=
LDLIBS :=

# Debug flags
DEBUGFLAGS := -g -DDEBUG

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    RM := del /Q
    EXE := .exe
    LDLIBS += -lsetupapi
else
    RM := rm -f
    EXE :=
    LDLIBS +=
endif

# Directories
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin
TESTDIR := tests

# Source files
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Main program
TARGET := $(BINDIR)/led_control$(EXE)

# Test files
TEST_SRCS := $(wildcard $(TESTDIR)/*.c)
TEST_OBJS := $(TEST_SRCS:$(TESTDIR)/%.c=$(OBJDIR)/%.o)
TEST_BINS := $(TEST_SRCS:$(TESTDIR)/%.c=$(BINDIR)/%)

# Phony targets
.PHONY: all clean debug test install uninstall help

# Default target
all: dirs $(TARGET)

# Create necessary directories
dirs:
	@mkdir -p $(BINDIR) $(OBJDIR)

# Build main program
$(TARGET): $(OBJS)
	@echo "Building $(TARGET)..."
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)
	@echo "Build complete!"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Debug build
debug: CFLAGS += $(DEBUGFLAGS)
debug: clean all

# Build and run tests
test: dirs $(TEST_BINS)
	@echo "Running tests..."
	@for test in $(TEST_BINS) ; do \
		echo "Running $$test..." ; \
		./$$test || exit 1 ; \
	done
	@echo "All tests passed!"

# Build test binaries
$(BINDIR)/%: $(OBJDIR)/%.o $(filter-out $(OBJDIR)/main.o, $(OBJS))
	@echo "Building test $@..."
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Compile test files
$(OBJDIR)/%.o: $(TESTDIR)/%.c
	@echo "Compiling test $<..."
	$(CC) $(CPPFLAGS) $(CFLAGS) -I$(TESTDIR) -c $< -o $@

# Install the program
install: all
	@echo "Installing program..."
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin
	@echo "Installation complete!"

# Uninstall the program
uninstall:
	@echo "Uninstalling program..."
	$(RM) $(DESTDIR)/usr/local/bin/$(notdir $(TARGET))
	@echo "Uninstall complete!"

# Clean build files
clean:
	@echo "Cleaning build files..."
	$(RM) $(OBJDIR)/*
	$(RM) $(BINDIR)/*
	@echo "Clean complete!"

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  test     - Build and run tests"
	@echo "  install  - Install the program"
	@echo "  uninstall- Remove the installed program"
	@echo "  clean    - Remove built files"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  CC       - $(CC)"
	@echo "  CFLAGS   - $(CFLAGS)"
	@echo "  LDLIBS   - $(LDLIBS)"