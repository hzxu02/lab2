/*
 * 7장: 파일 처리
 * 표준 출력 재지향 예제 (file_output.c)
 *  - dup2()를 사용해 stdout(1)을 지정한 파일로 연결
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // write, close, dup2
#include <fcntl.h>      // open, O_* flags
#include <sys/stat.h>   // mode_t

int main(int argc, char *argv[]) {
    int fd;                         // 파일 디스크립터
    const char *buf = "This is a test output file.\n";
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // 0644

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 1) 출력 파일 열기/생성
    fd = open(argv[1], flags, mode);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 2) stdout(1)을 fd로 재지향
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);                  // 실패 시 열린 fd 정리
        exit(EXIT_FAILURE);
    }

    // 3) 원래 fd는 더 이상 불필요하므로 닫기
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    // 4) 이제 write(1, ...)는 터미널이 아닌 argv[1] 파일로 기록됨
    if (write(STDOUT_FILENO, buf, strlen(buf)) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

