#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // opendir, readdir, closedir 함수를 위해 필요
#include <sys/stat.h> // stat 함수를 위해 필요
#include <unistd.h>   // access 함수를 위해 필요
#include <errno.h>    // errno 변수를 위해 필요
#include <string.h>   // strerror 함수를 위해 필요

int main() {
    const char *dir_path = "."; // 현재 디렉토리를 대상으로 함
    DIR *dir_stream;
    struct dirent *dir_entry;
    struct stat file_stat;

    // 1. opendir: 디렉토리 열기
    dir_stream = opendir(dir_path);
    if (dir_stream == NULL) {
        fprintf(stderr, "오류: 디렉토리 열기 실패 (%s): %s\n", dir_path, strerror(errno));
        return 1;
    }

    printf("현재 디렉토리('.') 항목 검사:\n");
    printf("==================================================\n");

    // 2. readdir: 디렉토리 항목 순회
    while ((dir_entry = readdir(dir_stream)) != NULL) {
        
        printf("%-20s | ", dir_entry->d_name); // d_name: 파일(항목) 이름

        // 3. stat: 파일의 상세 정보 (메타데이터) 가져오기
        //    stat은 파일 타입을 알아내기 위해 필요합니다.
        if (stat(dir_entry->d_name, &file_stat) == -1) {
            fprintf(stderr, "stat 오류: %s\n", strerror(errno));
            continue;
        }

        // 4. 파일 타입 확인 (S_ISDIR, S_ISREG)
        if (S_ISDIR(file_stat.st_mode)) {
            printf("%-10s | ", "디렉토리");
        } else if (S_ISREG(file_stat.st_mode)) {
            printf("%-10s | ", "일반 파일");
        } else {
            printf("%-10s | ", "기타");
        }

        // 5. access: 파일 권한 확인 (R_OK: 읽기 가능 여부)
        if (access(dir_entry->d_name, R_OK) == 0) {
            printf("읽기 가능\n");
        } else {
            printf("읽기 불가능\n");
        }
    }

    // 6. closedir: 디렉토리 닫기
    closedir(dir_stream);

    return 0;
}
