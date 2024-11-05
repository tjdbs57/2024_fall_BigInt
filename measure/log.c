#include "log.h"

void log_message(const char *message) {
    FILE *log_file = fopen("measure/log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    // 현재 시간 가져오기
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    // 시간과 메시지 출력
    fprintf(log_file, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
            local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec,
            message);

    fclose(log_file);
}

void measure_time()
{
    log_message("Test vector creation started.");

    // 시작 시간 기록
    clock_t start = clock();
    
    // 테스트 벡터 생성 함수 호출
    test_add();
    
    // 종료 시간 기록
    clock_t end = clock();
    
    // 실행 시간 계산
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;

    // 실행 시간 로그 기록
    char time_message[100];
    snprintf(time_message, sizeof(time_message), "Test vector creation completed in %f seconds.", seconds);
    log_message(time_message);
}