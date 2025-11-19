#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    int fd[2]; // 파이프의 파일 디스크립터를 저장할 배열 (0: 읽기용, 1: 쓰기용)
    pid_t pid;
    char write_msg[] = "Hello, Child Process! This is Parent.";
    char read_msg[BUFFER_SIZE];

    // 1. 파이프 생성
    // pipe() 함수는 두 개의 파일 디스크립터를 생성하여 fd 배열에 저장
    if (pipe(fd) == -1) {
        perror("Pipe 생성 실패");
        return 1;
    }

    // 2. 프로세스 복제 (fork)
    pid = fork();

    if (pid < 0) {
        perror("Fork 실패");
        return 1;
    }

    if (pid > 0) {
        // ===========================
        // 부모 프로세스 (데이터 쓰기)
        // ===========================
        
        // 부모는 데이터를 쓰기만 하므로, 읽기용 파이프 끝(fd[0])을 닫음 
        close(fd[0]); 

        printf("[부모] 파이프로 메시지를 보냅니다: \"%s\"\n", write_msg);
        
        // 파이프의 쓰기용 끝(fd[1])에 데이터를 씀
        write(fd[1], write_msg, strlen(write_msg) + 1); 
        
        // 쓰기가 끝나면 쓰기용 파이프 끝을 닫음 
        close(fd[1]);

        // 자식 프로세스가 종료될 때까지 기다림 
        wait(NULL);
    } 
    else {
        // ===========================
        // 자식 프로세스 (데이터 읽기)
        // ===========================
        
        // 자식은 데이터를 읽기만 하므로, 쓰기용 파이프 끝(fd[1])을 닫음 
        close(fd[1]); 

        // 파이프의 읽기용 끝(fd[0])에서 데이터를 읽어옴 
        read(fd[0], read_msg, BUFFER_SIZE);
        
        printf("[자식] 파이프에서 메시지를 받았습니다: \"%s\"\n", read_msg);
        
        // 읽기가 끝나면 읽기용 파이프 끝을 닫음 
        close(fd[0]);
    }

    return 0;
}
