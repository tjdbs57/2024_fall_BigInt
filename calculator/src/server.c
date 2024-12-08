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

    // bi_to_string 구현
    char temp[BUFFER_SIZE];  // 문자열을 저장할 임시 버퍼
    int i, len = 0;

    // 초기화
    temp[0] = '\0';

    // bigint 값에 따라 문자열로 변환
    for (i = x->wordlen - 1; i >= 0; i--) {
        char buf[64];
        sprintf(buf, "%lx", x->a[i]);  // 16진수로 변환 (word 단위)
        strcat(temp, buf);            // 임시 버퍼에 이어붙이기
    }

    // temp 문자열을 결과 변수로 복사
    strncpy(result, temp, BUFFER_SIZE);
}

int main() {


    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Diffie-Hellman 관련 변수
    bigint *p = NULL, *g = NULL, *b = NULL, *B = NULL, *A = NULL, *keyB = NULL;

    // 서버 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // 소켓 재사용 설정
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("setsockopt failed");
        exit(1);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // 서버 주소에 소켓 바인딩
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    // 클라이언트 연결 대기
    if (listen(sockfd, 1) == -1) {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }
    printf("Waiting for client connection...\n");

    // 클라이언트 연결 수락
    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock == -1) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    // 서버에서 시드 생성
    unsigned int seed = (unsigned int)time(NULL);  // 시드 값 생성
    srand(seed);
    printf("%x\n", seed);

    // 시드 값을 클라이언트로 전송
    char seed_buffer[BUFFER_SIZE];
    sprintf(seed_buffer, "%u", seed);
    send(client_sock, seed_buffer, strlen(seed_buffer), 0);

    // Diffie-Hellman 알고리즘 진행
    generate_large_prime(&p, 256, 25);  // 큰 소수 p 생성
    bi_set_by_string(&g, NON_NEGATIVE, "25", 16); // base g 생성

    // p와 g를 문자열로 변환
    char buffer_p[BUFFER_SIZE], buffer_g[BUFFER_SIZE];
    bi_to_string(p, buffer_p);
    bi_to_string(g, buffer_g);

    // p와 g를 하나의 버퍼에 담아 전송
    char combined_buffer[BUFFER_SIZE * 2];
    int written = snprintf(combined_buffer, sizeof(combined_buffer), "%s\n%s\n", buffer_p, buffer_g);
    if (written >= sizeof(combined_buffer)) {
        fprintf(stderr, "Error: combined buffer is too small!\n");
        close(client_sock);
        close(sockfd);
        exit(1);
    }

    if (send(client_sock, combined_buffer, written, 0) == -1) {
        perror("Failed to send p and g to client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }

    printf("Sent p and g successfully:\n%s", combined_buffer);

    // 개인 키 b 설정
    //bi_gen_rand(&b, NON_NEGATIVE, 1);
    bi_set_by_string(&b, NON_NEGATIVE, "f", 16);

    // 공개 키 B 계산
    exp_mod_montgomery(&g, &b, &B, &p);

    printf("Bob's public key B: ");
    bi_show_hex(B);

    // 클라이언트로부터 공개 키 A 수신
    char buffer[BUFFER_SIZE];
    int len = recv(client_sock, buffer, sizeof(buffer), 0);
    if (len < 0) {
        perror("Failed to receive data from client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }
    buffer[len] = '\0';
    bi_set_by_string(&A, NON_NEGATIVE, buffer, 16);

    printf("Received Alice's public key A: ");
    bi_show_hex(A);

    // 공개 키 B를 클라이언트로 전송
    char buffer1[BUFFER_SIZE];
    bi_to_string(B, buffer1);
    if (send(client_sock, buffer1, strlen(buffer1), 0) < 0) {
        perror("Failed to send Bob's public key to client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }


    // 비밀 키 계산

    exp_mod_montgomery(&A, &b, &keyB, &p);
    printf("Bob's computed secret key: ");
    bi_show_hex(keyB);

    // 메모리 해제 및 소켓 종료
    bi_delete(&p);
    bi_delete(&g);
    bi_delete(&b);
    bi_delete(&B);
    bi_delete(&A);
    bi_delete(&keyB);
    close(client_sock);
    close(sockfd);

    return 0;
}
