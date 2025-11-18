#include <stdio.h>
#include <stdlib.h>
#include <assert.h> // assert.h 헤더 파일 포함

int main(int argc, char *argv[]) {
    FILE *f;

    if (argc < 2) {
        printf("Usage: assert_use file_name\n");
        exit(1);
    }

    // if문 대신 assert()를 사용하여 파일 열기 성공 여부를 검사
    assert((f = fopen(argv[1], "r")) != NULL);

    printf("Open a file \"%s\".\n", argv[1]);
    fclose(f);
    return 0;
}
