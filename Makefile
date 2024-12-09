CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wextra -g -Iinclude -MMD

# Directories
SRCDIR 		= src
TESTDIR 	= test
MEASUREDIR 	= measure
INCDIR 		= include
OBJDIR 		= obj
BINDIR 		= bin

SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(TESTDIR)/*.c) $(wildcard $(MEASUREDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS := $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(OBJS))
OBJS := $(patsubst $(MEASUREDIR)/%.c,$(OBJDIR)/%.o,$(OBJS))
DEPS = $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	TARGET = $(BINDIR)/program.exe
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	RUN = $(BINDIR)\program.exe
	PYTHON_CMD := python
else
	TARGET = $(BINDIR)/program
	MKDIR = mkdir -p
	RMDIR = rm -rf
	RUN = ./$(TARGET)
	PYTHON_CMD := python3

endif

# Default target
all: dir $(TARGET)

dir:
	@$(MKDIR) $(OBJDIR) $(BINDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

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
	(cd test && $(PYTHON_CMD) cal.py)

measure:
	(cd bin && program > cycle_single.txt)
	(cd bin && move cycle_single.txt ../test/)
	(cd test && $(PYTHON_CMD) single_time.py)
compare:
	(cd bin && program > cycle.txt)
	(cd bin && move cycle.txt ../test/)
	(cd test && $(PYTHON_CMD) time.py)

.PHONY: all clean rebuild run verify check dir measure
