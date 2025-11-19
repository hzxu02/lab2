#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SHM_SIZE 1024
#define PATH_KEY "."
#define ID_KEY 'S'

// 세마포어 공용체 (일부 리눅스 버전 호환성 위함)
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// 공유 메모리 구조체
struct shm_data {
    int data_len;           // 읽은 데이터 길이
    char buffer[SHM_SIZE];  // 데이터 버퍼
    int eof;                // 파일 끝(EOF) 플래그
};

// 세마포어 조작 함수 (P연산: 대기, V연산: 신호)
void p(int semid, int sem_num) {
    struct sembuf buf = {sem_num, -1, 0};
    semop(semid, &buf, 1);
}

void v(int semid, int sem_num) {
    struct sembuf buf = {sem_num, 1, 0};
    semop(semid, &buf, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "사용법: %s <원본파일> <대상파일>\n", argv[0]);
        exit(1);
    }

    // 1. 키 생성
    key_t key = ftok(PATH_KEY, ID_KEY);
    if (key == -1) {
        perror("ftok 실패");
        exit(1);
    }

    // 2. 공유 메모리 생성 (shmget)
    int shmid = shmget(key, sizeof(struct shm_data), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget 실패");
        exit(1);
    }

    // 3. 공유 메모리 연결 (shmat)
    struct shm_data *shm_ptr = (struct shm_data *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat 실패");
        exit(1);
    }

    // 4. 세마포어 생성 (semget) - 2개의 세마포어 사용 (0: 읽기 가능, 1: 쓰기 가능)
    int semid = semget(key, 2, 0666 | IPC_CREAT);
    if (semid == -1) {
        perror("semget 실패");
        exit(1);
    }

    // 세마포어 초기화
    union semun u;
    u.val = 1; 
    semctl(semid, 0, SETVAL, u); // SEM 0 (부모 쓰기 권한): 1 (가능)
    u.val = 0;
    semctl(semid, 1, SETVAL, u); // SEM 1 (자식 읽기 권한): 0 (대기)

    // 5. 프로세스 분기
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork 실패");
        exit(1);
    }

    if (pid > 0) {
        // [부모 프로세스] 파일 읽기 -> 공유 메모리 쓰기
        FILE *src = fopen(argv[1], "rb");
        if (!src) { perror("원본 파일 열기 실패"); exit(1); }

        while (1) {
            p(semid, 0); // 부모 쓰기 권한 대기

            // 파일에서 데이터 읽기
            int len = fread(shm_ptr->buffer, 1, SHM_SIZE, src);
            shm_ptr->data_len = len;
            
            if (len < SHM_SIZE) {
                if (feof(src)) shm_ptr->eof = 1;
                else shm_ptr->eof = 0;
            } else {
                shm_ptr->eof = 0;
            }

            v(semid, 1); // 자식 읽기 권한 신호

            if (shm_ptr->eof) break;
        }

        fclose(src);
        wait(NULL); // 자식 종료 대기

        // 자원 해제
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID, u);
        printf("파일 복사 완료 (부모 종료)\n");

    } else {
        // [자식 프로세스] 공유 메모리 읽기 -> 파일 쓰기
        FILE *dst = fopen(argv[2], "wb");
        if (!dst) { perror("대상 파일 열기 실패"); exit(1); }

        while (1) {
            p(semid, 1); // 자식 읽기 권한 대기

            // 파일에 쓰기
            fwrite(shm_ptr->buffer, 1, shm_ptr->data_len, dst);

            int is_eof = shm_ptr->eof; // EOF 상태 저장

            v(semid, 0); // 부모 쓰기 권한 신호

            if (is_eof) break;
        }

        fclose(dst);
        shmdt(shm_ptr); // 공유 메모리 해제
        exit(0);
    }

    return 0;
}
