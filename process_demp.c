#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // fork, exec, getpid 등을 위한 헤더
#include <sys/types.h>
#include <sys/wait.h> // wait 함수를 위한 헤더

int main() {
    pid_t pid;

    printf("=== 프로세스 생성 및 실행 예제 ===\n");
    printf("부모 프로세스 시작 (PID: %d)\n", getpid());

    // 1. fork(): 새로운 자식 프로세스 생성
    // - 부모에게는 자식의 PID를 반환
    // - 자식에게는 0을 반환
    // - 실패 시 -1 반환
    pid = fork();

    if (pid < 0) {
        // fork 실패
        perror("fork 실패");
        exit(1);
    } 
    else if (pid == 0) {
        // ===========================
        // 자식 프로세스 영역 (Child)
        // ===========================
        printf("[자식] 저는 자식 프로세스입니다. (PID: %d, PPID: %d)\n", getpid(), getppid());
        printf("[자식] 'ls -l' 명령어를 실행하고 종료합니다.\n");
        
        // 2. execlp(): 현재 프로세스의 이미지를 'ls' 프로그램으로 교체
        // execlp(실행할 파일, arg0, arg1, ..., NULL)
        execlp("ls", "ls", "-l", NULL);

        // exec 함수가 성공하면 이 아래 코드는 실행되지 않음 (메모리가 교체되므로)
        perror("[자식] exec 실패"); // 만약 여기까지 오면 exec 실패임
        exit(1);
    } 
    else {
        // ===========================
        // 부모 프로세스 영역 (Parent)
        // ===========================
        printf("[부모] 자식 프로세스(PID: %d)가 생성되었습니다.\n", pid);
        printf("[부모] 자식이 종료될 때까지 기다립니다...\n");

        // 3. wait(): 자식 프로세스가 종료될 때까지 대기 (동기화)
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("[부모] 자식 프로세스가 정상적으로 종료되었습니다. (종료 코드: %d)\n", WEXITSTATUS(status));
        } else {
            printf("[부모] 자식 프로세스가 비정상적으로 종료되었습니다.\n");
        }
        
        printf("[부모] 모든 작업 완료.\n");
    }

    return 0;
}
