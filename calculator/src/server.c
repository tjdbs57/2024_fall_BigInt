#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "bigint.h"
#include "prime.h"
#include "arithmetic.h"

#define BUFFER_SIZE     8192

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


// Bob (서버)
int main() {

    int sockfd, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Diffie-Hellman 관련 변수
    bigint *p = NULL, *g = NULL, *b = NULL, *B = NULL, *A = NULL, *keyB = NULL;

    // 1. 서버 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. SO_REUSEADDR 설정 (소켓 재사용)
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        perror("setsockopt failed");
        exit(1);
    }

    // 2. 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // 포트 번호 8080

    // 3. 서버 주소에 소켓 바인딩
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    // 4. 클라이언트 연결 대기
    if (listen(sockfd, 1) == -1) {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }
    printf("Waiting for client connection...\n");

    // 5. 클라이언트 연결 수락
    client_sock = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock == -1) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    // 6. Diffie-Hellman 알고리즘 진행
    generate_large_prime(&p, 256, 5);  // 큰 소수 p 생성
    srand((u32)time(NULL));

    bi_gen_rand(&g, NON_NEGATIVE, 5);  // base g 생성
   //bi_show_hex(g);

    // 8. p와 g를 클라이언트에게 전송
    char buffer_p[BUFFER_SIZE], buffer_g[BUFFER_SIZE];

    // 송신 후, 버퍼 초기화
    memset(buffer_p, 0, sizeof(buffer_p));  // buffer_p를 0으로 초기화
    memset(buffer_g, 0, sizeof(buffer_g));  // buffer_g를 0으로 초기화
    bi_to_string(p, buffer_p);  // 소수 p를 문자열로 변환
    bi_to_string(g, buffer_g);  // 베이스 g를 문자열로 변환
    printf("%s", buffer_g);
    // 소수 p 전송
    int sent_p = send(client_sock, buffer_p, strlen(buffer_p), 0);
    if (sent_p < 0) {
        perror("Failed to send p to client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }
    buffer_p[sent_p] = '\0';
    
    // 베이스 g 전송
    int sent_g = send(client_sock, buffer_g, strlen(buffer_g), 0);
    if (sent_g < 0) {
        perror("Failed to send g to client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }
    buffer_g[sent_g] = '\0';


    //printf("send finish!\n");

    bi_set_by_string(&b, NON_NEGATIVE, "f", 16);  // Bob의 개인 키 선택
    exp_mod_montgomery(&g, &b, &B, &p);  // Bob의 공개 키 B 계산

    printf("Bob's public key B: ");
    bi_show_hex(B);  // 16진수 출력

    // 7. Alice로부터 공개 키 A를 수신
    char buffer[BUFFER_SIZE];
    int len = recv(client_sock, buffer, sizeof(buffer), 0);
    if (len < 0) {
        perror("Failed to receive data from client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }
    buffer[len] = '\0';  // 문자열 종료
    bi_set_by_string(&A,NON_NEGATIVE, buffer, 16);  // Alice의 공개 키 A 생성

    printf("Received Alice's public key A: ");
    bi_show_hex(A);


    // 서버에서 Bob의 공개 키 B를 클라이언트로 전송
    char buffer1[BUFFER_SIZE];  // 공개 키를 보낼 버퍼
    bi_to_string(B, buffer1);  // Bob의 공개 키를 문자열로 변환

    // 공개 키 B를 클라이언트로 전송
    int sent_bytes = send(client_sock, buffer1, strlen(buffer1), 0);
    if (sent_bytes < 0) {
        perror("Failed to send Bob's public key to client");
        close(client_sock);
        close(sockfd);
        exit(1);
    }

    printf("Sent Bob's public key B: ");
    bi_show_hex(B);  // 송신된 공개 키 출력
    
    // 8. Bob의 비밀 키를 사용하여 비밀 키 계산 (keyB = A^b % p)
    exp_mod_montgomery(&A, &b, &keyB, &p);

    printf("Bob's computed secret key: ");
    bi_show_hex(keyB);  // 16진수 출력

    // 10. 메모리 해제
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