#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <signal.h>

#define MAX_TEXT 512
#define KEY_PATH "."      // 키 생성을 위한 경로
#define KEY_ID 'C'        // 키 생성을 위한 문자

// 메시지 구조체 정의
struct message {
    long msg_type;        // 메시지 타입 (필수)
    char msg_text[MAX_TEXT]; // 메시지 내용
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "사용법: %s <사용자ID (1 또는 2)>\n", argv[0]);
        exit(1);
    }

    int user_id = atoi(argv[1]);
    if (user_id != 1 && user_id != 2) {
        fprintf(stderr, "사용자 ID는 1 또는 2여야 합니다.\n");
        exit(1);
    }

    // 송신 타입과 수신 타입 결정
    // 사용자 1: 송신(1) -> 수신(2)
    // 사용자 2: 송신(2) -> 수신(1)
    long send_type = user_id;
    long recv_type = (user_id == 1) ? 2 : 1;

    // 1. 고유 키 생성 (ftok)
    key_t key = ftok(KEY_PATH, KEY_ID);
    if (key == -1) {
        perror("ftok 실패");
        exit(1);
    }

    // 2. 메시지 큐 생성 또는 가져오기 (msgget)
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget 실패");
        exit(1);
    }

    printf("=== 채팅 프로그램 (사용자 %d) ===\n", user_id);
    printf("메시지 큐 ID: %d\n", msgid);
    printf("'exit'를 입력하면 종료합니다.\n\n");

    // 3. 프로세스 분기 (fork)
    // 자식 프로세스: 키보드 입력 -> 메시지 송신
    // 부모 프로세스: 메시지 수신 -> 화면 출력
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork 실패");
        exit(1);
    }

    if (pid == 0) {
        // [자식 프로세스] 메시지 송신 담당
        struct message msg;
        msg.msg_type = send_type;

        while (1) {
            // 키보드 입력 받기
            if (fgets(msg.msg_text, MAX_TEXT, stdin) == NULL) break;

            // 'exit' 입력 시 종료
            if (strncmp(msg.msg_text, "exit", 4) == 0) {
                // 상대방에게도 종료 알림을 보내고 싶다면 전송 후 종료
                if (msgsnd(msgid, &msg, strlen(msg.msg_text) + 1, 0) == -1) {
                    perror("msgsnd 실패");
                }
                break;
            }

            // 4. 메시지 전송 (msgsnd)
            if (msgsnd(msgid, &msg, strlen(msg.msg_text) + 1, 0) == -1) {
                perror("msgsnd 실패");
                exit(1);
            }
        }
        printf("채팅을 종료합니다 (송신).\n");
        exit(0);
    } 
    else {
        // [부모 프로세스] 메시지 수신 담당
        struct message msg;

        while (1) {
            // 5. 메시지 수신 (msgrcv)
            // recv_type에 해당하는 메시지만 기다림
            if (msgrcv(msgid, &msg, MAX_TEXT, recv_type, 0) == -1) {
                perror("msgrcv 실패");
                break;
            }

            // 상대방이 'exit'를 보냈으면 종료
            if (strncmp(msg.msg_text, "exit", 4) == 0) {
                printf("\n상대방이 채팅을 종료했습니다.\n");
                break;
            }

            printf("[상대방]: %s", msg.msg_text);
        }
        
        // 자식 프로세스 종료 대기 및 메시지 큐 삭제는 생략
        // 터미널에서 'ipcrm -q <msgid>' 명령으로 큐를 삭제할 수 있음 
        kill(pid, SIGTERM); // 자식 프로세스 종료
        exit(0);
    }

    return 0;
}
