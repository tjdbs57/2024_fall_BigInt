#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "bigint.h"
#include "prime.h"
#include "arithmetic.h"

#define BUFFER_SIZE 8192

// bi_to_string 함수 정의

void bi_to_string(bigint *x, char *result) {
    if (x == NULL || result == NULL) {
        return;
    }

    // bi_to_string 구현
    char temp[BUFFER_SIZE];  // 문자열을 저장할 임시 버퍼
    int i, len = 0;
    
    // 초기화
    temp[0] = '\0';

    // bigint 값에 따라 문자열로 변환
    for (i = x->wordlen - 1; i >= 0; i--) {
        // x->a[i]는 word 단위로 저장된 값이라 이를 문자열로 변환
        char buf[64];
        sprintf(buf, "%lx", x->a[i]);  // 16진수로 변환 (word 단위)
        strcat(temp, buf);  // 임시 버퍼에 이어붙이기
    }

    // temp 문자열을 결과 변수로 복사
    strncpy(result, temp, BUFFER_SIZE);
}

// Alice (클라이언트)
int main() {
    srand((u32)time(NULL));

    int sockfd;
    struct sockaddr_in server_addr;
    char *server_ip = "127.0.0.1";  // 서버 IP
    int server_port = 8080;         // 서버 포트

    // Diffie-Hellman 관련 변수
    bigint *p = NULL, *g = NULL, *a = NULL, *A = NULL, *B = NULL, *keyA = NULL;

    // 1. 클라이언트 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // 3. 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        close(sockfd);
        exit(1);
    }

    // 4. 서버로부터 소수 p와 베이스 g 수신
    char buffer_p[BUFFER_SIZE], buffer_g[BUFFER_SIZE];
// 소수 p 수신
    int len_p = recv(sockfd, buffer_p, sizeof(buffer_p), 0);
    if (len_p < 0) {
        perror("Failed to receive p from server");
        close(sockfd);
        exit(1);
    }
    buffer_p[len_p] = '\0';  // 수신된 데이터 종료 처리
    printf("Received p: %s\n", buffer_p);  // 수신된 p 출력
    
    // 베이스 g 수신
    int len_g = recv(sockfd, buffer_g, sizeof(buffer_g), 0);
    if (len_g < 0) {
        perror("Failed to receive g from server");
        close(sockfd);
        exit(1);
    }
    buffer_g[len_g] = '\0';  // 수신된 데이터 종료 처리
    printf("Received g: %s\n", buffer_g);  // 수신된 g 출력

    // 소수 p와 베이스 g를 bigint로 변환
    bi_set_by_string(&p, NON_NEGATIVE, buffer_p, 16);  // 소수 p 설정
    bi_set_by_string(&g, NON_NEGATIVE, buffer_g, 16);  // 베이스 g 설정

    printf("Received p: ");
    bi_show_hex(p);  // 수신된 소수 p 출력
    printf("Received g: ");
    bi_show_hex(g);  // 수신된 베이스 g 출력

    // 5. Diffie-Hellman 알고리즘 진행
    bi_set_by_string(&a, NON_NEGATIVE, "10", 16);  // Alice의 개인 키 선택
    exp_mod_montgomery(&g, &a, &A, &p);  // Alice의 공개 키 A 계산

    printf("Alice's public key A: ");
    bi_show_hex(A);  // 16진수 출력

    // 5. Bob에게 공개 키 A 전송
    char buffer[BUFFER_SIZE];
    bi_to_string(A, buffer);  // bi_to_string은 A 값을 문자열로 변환하여 buffer에 저장
    send(sockfd, buffer, strlen(buffer), 0);  // buffer의 내용을 전송

    // 6. Bob의 공개 키 B 수신
    char buffer1[BUFFER_SIZE];  // 수신할 데이터를 담을 버퍼
    int len = recv(sockfd, buffer1, sizeof(buffer1), 0);  // 데이터를 수신
    
    if (len < 0) {
        perror("Failed to receive data from server");
        close(sockfd);
        exit(1);
    }
    
    buffer1[len] = '\0';  // 문자열 종료 처리
    
    // Bob의 공개 키 B 출력
    bi_set_by_string(&B, NON_NEGATIVE, buffer1, 16);  // 16진수로 변환하여 bigint로 저장
    printf("Received Bob's public key B: ");
    bi_show_hex(B);  // Bob의 공개 키 B 출력

    // 7. Alice의 비밀 키를 사용하여 비밀 키 계산 (keyA = B^a % p)
    exp_mod_montgomery(&B, &a, &keyA, &p);

    printf("Alice's computed secret key: ");
    bi_show_hex(keyA);  // 16진수 출력

    // 8. 메모리 해제
    bi_delete(&p);
    bi_delete(&g);
    bi_delete(&a);
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&keyA);
    close(sockfd);

    return 0;
}
