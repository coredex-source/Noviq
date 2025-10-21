# Noviq Interpreter Makefile

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -Iinclude
LDFLAGS = -lm
TARGET = noviq
SRCDIR = src
OBJDIR = obj

# Source files
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/interpreter/interpreter.c \
          $(SRCDIR)/interpreter/variables.c \
          $(SRCDIR)/interpreter/expressions.c \
          $(SRCDIR)/operators/arithmetic.c \
          $(SRCDIR)/operators/logical.c \
          $(SRCDIR)/operators/comparison.c \
          $(SRCDIR)/statements/display.c \
          $(SRCDIR)/statements/input.c \
          $(SRCDIR)/statements/control_flow.c \
          $(SRCDIR)/utils/error.c

# Object files
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Create object directory structure
$(shell mkdir -p $(OBJDIR)/interpreter $(OBJDIR)/operators $(OBJDIR)/statements $(OBJDIR)/utils)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET) $(TARGET).exe
	@echo "Clean complete"

rebuild: clean all

.PHONY: all clean rebuild