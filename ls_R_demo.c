#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

void list_directory_recursive(const char *base_path) {
    DIR *dir_stream;
    struct dirent *dir_entry;
    struct stat file_stat;
    
    char *subdirs[1024]; 
    int subdir_count = 0;
    
    char current_path[1024];

    if ((dir_stream = opendir(base_path)) == NULL) {
        fprintf(stderr, "오류: %s 디렉토리를 열 수 없음: %s\n", base_path, strerror(errno));
        return;
    }

    printf("\n%s:\n", base_path);

    // 1차 탐색 (이름 출력 및 하위 디렉토리 저장)
    while ((dir_entry = readdir(dir_stream)) != NULL) {
        
        // 수정된 부분: 이름이 '.'으로 시작하는 모든 항목 무시
        if (dir_entry->d_name[0] == '.') {
            continue;
        }

        printf("%s\t", dir_entry->d_name);

        snprintf(current_path, sizeof(current_path), "%s/%s", base_path, dir_entry->d_name);
        
        if (stat(current_path, &file_stat) == 0) {
            if (S_ISDIR(file_stat.st_mode)) {
                if (subdir_count < 1024) {
                    subdirs[subdir_count++] = strdup(current_path);
                }
            }
        }
    }
    printf("\n"); 
    closedir(dir_stream);

    // 2차 탐색 (저장된 하위 디렉토리 재귀 호출)
    for (int i = 0; i < subdir_count; i++) {
        list_directory_recursive(subdirs[i]); 
        free(subdirs[i]); 
    }
}

int main(int argc, char *argv[]) {
    const char *start_path = (argc > 1) ? argv[1] : ".";
    list_directory_recursive(start_path);
    return 0;
}
