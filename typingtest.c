/*
 * 7장 파일 처리
 * 파일 이름: typingtest.c
 */

#include <stdio.h>
#include <termios.h>     // 터미널 제어
#include <sys/types.h>
#include <fcntl.h>       // open()
#include <unistd.h>      // read(), write(), close()

#define PASSWORDSIZE 12

int main(void)
{
    int fd;
    int nread, cnt = 0, errcnt = 0;
    char ch, text[] = "The magic thing is that you can change it.";
    struct termios init_attr, new_attr;

    // 1) 표준 입력(fd=0)을 터미널 파일로 열기 (읽기/쓰기)
    fd = open(ttyname(fileno(stdin)), O_RDWR);

    // 2) 기존 터미널 속성 저장
    tcgetattr(fd, &init_attr);

    // 3) 새 터미널 속성 설정 (비정규 모드 + 입력 글자 숨기기)
    new_attr = init_attr;
    new_attr.c_lflag &= ~ICANON;   // Canonical 모드 해제 (Enter 없이 바로 입력 처리)
    new_attr.c_lflag &= ~ECHO;     // 입력한 문자를 화면에 출력하지 않음
    new_attr.c_cc[VMIN] = 1;       // 최소 1글자 입력되면 read()가 반환
    new_attr.c_cc[VTIME] = 0;      // read() 대기 시간 없음

    if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
        fprintf(stderr, "터미널 속성을 설정할 수 없습니다.\n");
        return 1;
    }

    // 4) 타자 연습 안내 출력
    printf("다음 문장을 그대로 입력하세요:\n%s\n", text);

    // 5) 문자 하나씩 읽으며 비교
    while ((nread = read(fd, &ch, 1)) > 0 && ch != '\n') {
        if (ch == text[cnt++]) {       // 맞게 입력
            write(fd, &ch, 1);
        } else {                        // 틀리면 * 표시
            write(fd, "*", 1);
            errcnt++;
        }
    }

    // 6) 결과 출력
    printf("\n타이핑 오류의 횟수는 %d\n", errcnt);

    // 7) 터미널 속성 원래대로 복구
    tcsetattr(fd, TCSANOW, &init_attr);
    close(fd);

    return 0;
}

