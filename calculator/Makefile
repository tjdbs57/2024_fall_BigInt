CC = gcc
CFLAGS = -O3 -std=c99 -Wall -Wextra -g -I../include  # 헤더 파일 경로를 포함
LDFLAGS = -L../lib -lbigint  # lib 디렉토리 경로와 라이브러리 이름을 지정
SRC = calculator.c
OBJ = calculator.o
TARGET = calculator_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

run: $(TARGET)
	./$(TARGET) 
	
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
