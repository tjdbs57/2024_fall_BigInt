CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Iinclude -MMD

# Directories
SRCDIR = src
TESTDIR = test
INCDIR = include
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(TESTDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS := $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(OBJS))
DEPS = $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	TARGET = $(BINDIR)/program.exe
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	RUN = $(TARGET)
else
	TARGET = $(BINDIR)/program
	MKDIR = mkdir -p
	RMDIR = rm -rf
	RUN = ./$(TARGET)
endif

# Default target
all: dir $(TARGET)

dir:
	@$(MKDIR) $(OBJDIR) $(BINDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	$(RMDIR) $(OBJDIR) $(BINDIR)

rebuild: clean all

run: $(TARGET)
	$(RUN)

.PHONY: all clean rebuild run
