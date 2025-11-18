

#include <stdio.h>
#include "calc.h"

int main() {
    printf("--- 정적 라이브러리 테스트 ---\n");
    printf("10 + 5 = %d\n", add(10, 5));
    printf("10 - 5 = %d\n", subtract(10, 5));
    printf("10 * 5 = %d\n", multiply(10, 5));
    printf("10 / 5 = %.1f\n", divide(10, 5));


// 시간을 소모하기 위한 반복문 추가
    printf("Running a busy loop to consume time...\n");
    for (long i = 0; i < 2000000000; i++) {
        // Just wasting CPU time
    }
    printf("Loop finished.\n");


    return 0;
}
