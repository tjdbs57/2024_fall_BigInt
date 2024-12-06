CC = gcc
CFLAGS = -Wall -I../include
LDFLAGS = -L. -lbigint -ldl

# 소스 파일 및 타겟 정의
SRC = main.c
OBJ = main.o
TARGET = a.exe

# 컴파일 규칙
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

# 소스 파일을 오브젝트 파일로 컴파일
$(OBJ): $(SRC)
	$(CC) -c $(SRC) $(CFLAGS)

# 기본 클린 규칙
clean:
	rm -f $(TARGET) $(OBJ)
