CC = gcc

# CFLAGS에 헤더 파일 경로 추가
CFLAGS = -O3 -std=c99 -Wall -Wextra -g  -I../include -MMD
LDFLAGS = -L../lib
LIBS = -lbigint_64bit

SRCDIR = src
OBJDIR = obj
BINDIR = bin

# SRCS 변수가 src 디렉토리의 .c 파일을 잘 찾는지 확인
SRCS = $(wildcard $(SRCDIR)/*.c)  # src 디렉토리 내의 모든 .c 파일을 가져옴
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))  # .c 파일을 .o 파일로 변환
DEPS = $(OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	TARGET = $(BINDIR)/DH.exe
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	RUN = $(BINDIR)\DH.exe
else
	TARGET = $(BINDIR)/DH
	MKDIR = mkdir -p
	RMDIR = rm -rf
	RUN = ./$(TARGET)
endif

all: dir $(TARGET)

# 디렉토리 생성
dir:
	@$(MKDIR) $(OBJDIR) $(BINDIR)

# 타겟 생성
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

# .o 파일 생성
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# clean 명령어 수정
clean:
	$(RMDIR) $(OBJDIR) $(BINDIR)

# 빌드 후 실행하는 타겟
run: $(TARGET)
	$(RUN)

rebuild: clean all

valgrind:
	(valgrind --leak-check=yes ./bin/DH)
