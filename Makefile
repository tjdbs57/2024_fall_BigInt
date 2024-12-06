CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wextra -g -Iinclude -MMD -fPIC
LDFLAGS = -shared

# 워드 크기별 CFLAGS
CFLAGS_8BIT = -DWORDFLAG=8
CFLAGS_32BIT = -DWORDFLAG=32
CFLAGS_64BIT = -DWORDFLAG=64

# 워드 크기별 라이브러리 이름
LIBRARY_NAME_8BIT = bigint_8bit.so
LIBRARY_NAME_32BIT = bigint_32bit.so
LIBRARY_NAME_64BIT = bigint_64bit.so

# Directories
SRCDIR = src
TESTDIR = test
INCDIR = include
OBJDIR = obj
BINDIR = bin
LIBDIR = lib

# 소스 파일 목록
SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(TESTDIR)/*.c) $(wildcard $(MEASUREDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS := $(patsubst $(TESTDIR)/%.c,$(OBJDIR)/%.o,$(OBJS))
DEPS = $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	TARGET = $(BINDIR)/program.exe
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	RUN = $(BINDIR)\program.exe
	PYTHON_CMD := python
	LIBTARGET_8BIT = $(LIBDIR)/bigint_8bit.dll
	LIBTARGET_32BIT = $(LIBDIR)/bigint_32bit.dll
	LIBTARGET_64BIT = $(LIBDIR)/bigint_64bit.dll
else
	TARGET = $(BINDIR)/program
	MKDIR = mkdir -p
	RMDIR = rm -rf
	RUN = ./$(TARGET)
	PYTHON_CMD := python3
	LIBTARGET_8BIT = $(LIBDIR)/$(LIBRARY_NAME_8BIT)
	LIBTARGET_32BIT = $(LIBDIR)/$(LIBRARY_NAME_32BIT)
	LIBTARGET_64BIT = $(LIBDIR)/$(LIBRARY_NAME_64BIT)
endif

# Default target
all: dir $(TARGET) $(LIBTARGET_8BIT) $(LIBTARGET_32BIT) $(LIBTARGET_64BIT)

# 디렉토리 생성
dir:
	@$(MKDIR) $(OBJDIR) $(BINDIR) $(LIBDIR)

# 실행 파일 빌드
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LIBS)

# 8비트 라이브러리 빌드
$(LIBTARGET_8BIT): $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_8BIT) $(OBJS) -o $(LIBDIR)/$(LIBRARY_NAME_8BIT) $(LDFLAGS)

# 32비트 라이브러리 빌드
$(LIBTARGET_32BIT): $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_32BIT) $(OBJS) -o $(LIBDIR)/$(LIBRARY_NAME_32BIT) $(LDFLAGS)

# 64비트 라이브러리 빌드
$(LIBTARGET_64BIT): $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS_64BIT) $(OBJS) -o $(LIBDIR)/$(LIBRARY_NAME_64BIT) $(LDFLAGS)

# .c 파일을 .o 파일로 컴파일
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(MEASUREDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# 클린
clean:
	$(RMDIR) $(OBJDIR) $(BINDIR) $(LIBDIR)

# 리빌드
rebuild: clean all

# 실행
run: $(TARGET)
	$(RUN)

# 테스트 스크립트 실행
verify: $(TARGET)
	$(RUN) > ./test/test.txt
	(cd test && $(PYTHON_CMD) test.py)

check:
	(cd test && $(PYTHON_CMD) cal.py)

valgrind:
	(valgrind --leak-check=yes ./bin/program)

measure:
	(cd bin && program > cycle_single.txt)
	(cd bin && move cycle_single.txt ../test/)
	(cd test && $(PYTHON_CMD) time.py)

.PHONY: all clean rebuild run verify check dir measure
