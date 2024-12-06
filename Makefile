CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wextra -g -Iinclude -MMD -fPIC
LDFLAGS = -shared

# Directories
SRCDIR 		= src
TESTDIR 	= test
INCDIR 		= include
OBJDIR 		= obj
BINDIR 		= bin
LIBDIR		= lib



SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(TESTDIR)/*.c) 
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS := $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(OBJS))
DEPS = $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	TARGET = $(BINDIR)/program.exe
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	RUN = $(BINDIR)\program.exe
	PYTHON_CMD := python
	LIBTARGET = $(LIBDIR)/libbigint_32bit.dll


else
	TARGET = $(BINDIR)/program
	MKDIR = mkdir -p
	RMDIR = rm -rf
	RUN = ./$(TARGET)
	PYTHON_CMD := python3
	LIBTARGET = $(LIBDIR)/libbigint_64bit.so

endif

# Default target
all: dir $(TARGET) $(LIBTARGET)

dir:
	@$(MKDIR) $(OBJDIR) $(BINDIR) $(LIBDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

$(LIBTARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(MEASUREDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	$(RMDIR) $(OBJDIR) $(BINDIR)

rebuild: clean all

run: $(TARGET)
	$(RUN)

# Verify with test script
verify: $(TARGET)
	$(RUN) > ./test/test_all.txt
	(cd test && $(PYTHON_CMD) test_all.py)

check:
	(cd calculator && $(PYTHON_CMD) check.py)

valgrind:
	(valgrind --leak-check=yes ./bin/program)

measure:
	(cd bin && program > cycle_single.txt)
	(cd bin && move cycle_single.txt ../test/)
	(cd test && $(PYTHON_CMD) time.py)

.PHONY: all clean rebuild run verify check dir measure
