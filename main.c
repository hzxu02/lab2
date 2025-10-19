

#include <stdio.h>
#include "calc.h"

int main() {
    printf("--- 정적 라이브러리 테스트 ---\n");
    printf("10 + 5 = %d\n", add(10, 5));
    printf("10 - 5 = %d\n", subtract(10, 5));
    printf("10 * 5 = %d\n", multiply(10, 5));
    printf("10 / 5 = %.1f\n", divide(10, 5));
    return 0;
}
