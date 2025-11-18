#include <stdio.h>
#include <stdlib.h>
#include "my_assert.h" // 우리가 만든 assert 헤더 파일을 포함

// 나이의 유효성을 검사하는 함수
void check_age(int age) {
    // 나이는 0 이상 120 이하여야 한다는 조건을 검사
    my_assert(age >= 0 && age <= 120);
    
    printf("입력된 나이 %d는 유효한 범위입니다.\n", age);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s <나이>\n", argv[0]);
        return 1;
    }

    int age = atoi(argv[1]);
    check_age(age);

    return 0;
}
