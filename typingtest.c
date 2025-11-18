#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // 시간 측정을 위한 헤더
#include <unistd.h> // read 함수 등 (필요시)

#define NUM_SENTENCES 3

int main() {
    // 연습할 문장들
    const char *sentences[NUM_SENTENCES] = {
        "The quick brown fox jumps over the lazy dog.",
        "Linux is an open-source operating system.",
        "System programming involves low-level manipulation."
    };

    char input[256];
    int total_typos = 0;
    int total_chars = 0;
    double total_time = 0.0;

    // 랜덤 시드 초기화 (필요시 순서 섞기 위해 사용 가능)
    srand(time(NULL));

    printf("=== 타자 연습 프로그램 ===\n");
    printf("아래 문장을 그대로 입력하고 엔터를 누르세요.\n\n");

    printf("준비... 엔터를 누르면 시작합니다.");
    getchar(); // 엔터 입력 대기
    printf("\n");

    for (int i = 0; i < NUM_SENTENCES; i++) {
        printf("\n[문장 %d/%d]\n", i + 1, NUM_SENTENCES);
        printf("%s\n", sentences[i]);
        printf("입력: ");

        // 시작 시간 측정
        time_t start = time(NULL);

        // 사용자 입력 받기
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; 
        }

        // 종료 시간 측정
        time_t end = time(NULL);

        // fgets는 끝에 줄바꿈('\n')을 포함하므로 제거
        input[strcspn(input, "\n")] = 0;

        // 입력 길이 계산
        int len_sentence = strlen(sentences[i]);
        int len_input = strlen(input);
        int typos = 0;
        
        // 오타 계산 (길이 차이도 오타로 간주)
        int min_len = (len_sentence < len_input) ? len_sentence : len_input;
        
        for (int j = 0; j < min_len; j++) {
            if (sentences[i][j] != input[j]) {
                typos++;
            }
        }
        // 길이가 다르면 남은 글자 수도 오타에 포함
        typos += abs(len_sentence - len_input);

        // 통계 누적
        double duration = difftime(end, start);
        if (duration <= 0) duration = 1.0; // 0초 방지

        total_typos += typos;
        total_chars += len_input;
        total_time += duration;

        printf("-> 오타: %d개, 소요 시간: %.0f초\n", typos, duration);
    }

    // 최종 결과 계산 (WPM: Words Per Minute, 보통 5타를 1단어로 간주)
    // 분당 타수(CPM) = (총 글자수 / 총 초) * 60
    double cpm = (total_chars / total_time) * 60.0;
    
    printf("\n============================\n");
    printf("최종 결과:\n");
    printf("총 소요 시간: %.0f초\n", total_time);
    printf("총 오타 수  : %d개\n", total_typos);
    printf("평균 타수   : %.0f 타/분 (CPM)\n", cpm);
    printf("============================\n");

    return 0;
}
