#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> // 시그널 처리를 위한 헤더

// 1. SIGINT (Ctrl+C) 핸들러 함수
void handle_sigint(int sig) {
    printf("\n\n[Signal] SIGINT(%d)를 수신했습니다!\n", sig);
    printf("Ctrl+C를 눌렀지만, 바로 종료되지 않고 이 함수가 실행됩니다.\n");
    printf("계속 실행합니다...\n");
    // (보통은 여기서 자원을 정리하고 종료하거나, 다시 프롬프트를 띄웁니다.)
}

// 2. SIGALRM (알람) 핸들러 함수
void handle_sigalrm(int sig) {
    printf("\n\n[Signal] SIGALRM(%d)을 수신했습니다!\n", sig);
    printf("예약된 시간이 되었습니다. 프로그램을 정상 종료합니다.\n");
    exit(0); // 프로그램 종료
}

int main() {
    // 3. 시그널 핸들러 등록
    // SIGINT(Ctrl+C)가 발생하면 handle_sigint 함수 실행
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        perror("SIGINT 핸들러 등록 실패");
        exit(1);
    }

    // SIGALRM(알람)이 발생하면 handle_sigalrm 함수 실행
    if (signal(SIGALRM, handle_sigalrm) == SIG_ERR) {
        perror("SIGALRM 핸들러 등록 실패");
        exit(1);
    }

    printf("=== 시그널 처리 예제 ===\n");
    printf("10초 후에 알람에 의해 자동 종료됩니다.\n");
    printf("그 전에 Ctrl+C를 눌러 시그널이 어떻게 처리되는지 확인해보세요.\n\n");

    // 4. 알람 설정 (10초 후 SIGALRM 발생)
    alarm(10);

    // 무한 루프 (작업 중임을 가정)
    int count = 0;
    while (1) {
        printf("작업 수행 중... (%d초)\n", count++);
        sleep(1); // 1초 대기
    }

    return 0;
}
