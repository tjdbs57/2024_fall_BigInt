#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "bigint.h"
#include "prime.h"
#include "arithmetic.h"

#define BUFFER_SIZE 8192

void bi_to_string(bigint *x, char *result) {
    if (x == NULL || result == NULL) {
        return;
    }

    char temp[BUFFER_SIZE];  // 문자열을 저장할 임시 버퍼
    int i;

    // 초기화
    temp[0] = '\0';

    // bigint 값을 문자열로 변환
    for (i = x->wordlen - 1; i >= 0; i--) {
        char buf[64];
        sprintf(buf, "%lx", x->a[i]);  // 16진수로 변환
        strcat(temp, buf);
    }

    // 변환된 문자열 복사
    strncpy(result, temp, BUFFER_SIZE - 1);
    result[BUFFER_SIZE - 1] = '\0';  // NULL 종료
}

int main() {

    
    int sockfd;
    struct sockaddr_in server_addr;
    char *server_ip = "127.0.0.1";
    int server_port = 8080;

    bigint *p = NULL, *g = NULL, *a = NULL, *A = NULL, *B = NULL, *keyA = NULL;

    // 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // 서버 연결
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 클라이언트에서 시드 수신
    char seed_buffer[BUFFER_SIZE];
    int len0 = recv(sockfd, seed_buffer, sizeof(seed_buffer) - 1, 0);
    seed_buffer[len0] = '\0';  // 문자열 종료 처리

    // 받은 시드 값으로 srand 설정
    unsigned int seed = atoi(seed_buffer);  // 시드 값 정수로 변환
    srand(seed);  // 난수 생성기 초기화
    printf("%x\n", seed);


    // 서버로부터 p와 g 수신
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int len = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (len <= 0) {
        perror("Failed to receive p and g from server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[len] = '\0';

    // p와 g 분리
    char *token = strtok(buffer, "\n");
    char buffer_p[BUFFER_SIZE], buffer_g[BUFFER_SIZE];
    if (token != NULL) {
        strncpy(buffer_p, token, sizeof(buffer_p) - 1);
        buffer_p[sizeof(buffer_p) - 1] = '\0';
    }
    token = strtok(NULL, "\n");
    if (token != NULL) {
        strncpy(buffer_g, token, sizeof(buffer_g) - 1);
        buffer_g[sizeof(buffer_g) - 1] = '\0';
    }

    // p와 g를 bigint로 변환
    if (bi_set_by_string(&p, NON_NEGATIVE, buffer_p, 16) != 0 || 
        bi_set_by_string(&g, NON_NEGATIVE, buffer_g, 16) != 0) {
        fprintf(stderr, "Error: Invalid p or g received from server.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Received p: ");
    bi_show_hex(p);
    printf("Received g: ");
    bi_show_hex(g);

    // Alice의 개인 키 및 공개 키 계산
    bi_set_by_string(&a, NON_NEGATIVE, "10", 16);

    exp_mod_montgomery(&g, &a, &A, &p);

    printf("Alice's public key A: ");
    bi_show_hex(A);

    // 공개 키 A 전송
    char buffer_A[BUFFER_SIZE];
    bi_to_string(A, buffer_A);
    if (send(sockfd, buffer_A, strlen(buffer_A), 0) == -1) {
        perror("Failed to send Alice's public key to server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Bob의 공개 키 B 수신
    char buffer_B[BUFFER_SIZE];
    memset(buffer_B, 0, sizeof(buffer_B));
    len = recv(sockfd, buffer_B, sizeof(buffer_B) - 1, 0);
    if (len <= 0) {
        perror("Failed to receive Bob's public key");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer_B[len] = '\0';

    // Bob의 공개 키 B 변환
    if (bi_set_by_string(&B, NON_NEGATIVE, buffer_B, 16) != 0) {
        fprintf(stderr, "Error: Invalid B received from server.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Received Bob's public key B: ");
    bi_show_hex(B);

    // 비밀 키 계산
    exp_mod_montgomery(&B, &a, &keyA, &p);
    printf("Alice's computed secret key: ");
    bi_show_hex(keyA);

    // 메모리 해제 및 소켓 종료
    bi_delete(&p);
    bi_delete(&g);
    bi_delete(&a);
    bi_delete(&A);
    bi_delete(&B);
    bi_delete(&keyA);
    close(sockfd);

    return 0;
}
