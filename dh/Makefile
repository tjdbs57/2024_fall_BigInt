CC = gcc

# CFLAGS에 헤더 파일 경로 추가
CFLAGS = -O3 -std=c99 -Wall -Wextra -g -I../include -MMD
LDFLAGS = -L../lib
LIBS = -lbigint_64bit -lgmp

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SERVER_SRCS = server.c prime.c
CLIENT_SRCS = client.c prime.c
SERVER_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SERVER_SRCS))
CLIENT_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(CLIENT_SRCS))

SERVER_TARGET = $(BINDIR)/server
CLIENT_TARGET = $(BINDIR)/client

DEPS = $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d)

ifeq ($(OS),Windows_NT)
	MKDIR = mkdir
	RMDIR = rmdir /S /Q
	RUN_SERVER = $(SERVER_TARGET)
	RUN_CLIENT = $(CLIENT_TARGET)
else
	MKDIR = mkdir -p
	RMDIR = rm -rf
	RUN_SERVER = ./$(SERVER_TARGET)
	RUN_CLIENT = ./$(CLIENT_TARGET)
endif

all: dir $(SERVER_TARGET) $(CLIENT_TARGET)

# 디렉토리 생성
dir:
	@$(MKDIR) $(OBJDIR) $(BINDIR)

# 서버 실행 파일 생성
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $@ $(LDFLAGS) $(LIBS)

# 클라이언트 실행 파일 생성
$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@ $(LDFLAGS) $(LIBS)

# .o 파일 생성 규칙
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

# clean 명령어
clean:
	$(RMDIR) $(OBJDIR) $(BINDIR)

# 빌드 후 서버 실행
run_server: $(SERVER_TARGET)
	$(RUN_SERVER)

# 빌드 후 클라이언트 실행
run_client: $(CLIENT_TARGET)
	$(RUN_CLIENT)

rebuild: clean all
