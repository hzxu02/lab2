# 컴파일러 및 플래그 정의
CC = gcc
CFLAGS = -Wall -g -I./include -pg  # 디버깅을 위해 -g 옵션 추가
LDFLAGS = -L./lib -pg
LIBS = -lcalc

# 생성될 파일 및 디렉토리 이름 정의
TARGET_STATIC = main_static
TARGET_SHARED = main_shared
LIB_DIR = lib
OBJ_DIR = obj

# 소스 및 목적 파일 목록 자동 생성
LIB_SRCS = $(wildcard src/*.c)
LIB_OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(LIB_SRCS))
MAIN_OBJ = main.o

.PHONY: all clean

# 기본 목표: 정적 및 공유 실행 파일 모두 빌드
all: $(TARGET_STATIC) $(TARGET_SHARED)

# --- 실행 파일 빌드 규칙 ---
$(TARGET_STATIC): $(MAIN_OBJ) $(LIB_DIR)/libcalc.a
	$(CC) -static -o $@ $(MAIN_OBJ) $(LDFLAGS) $(LIBS)

$(TARGET_SHARED): $(MAIN_OBJ) $(LIB_DIR)/libcalc.so
	$(CC) -o $@ $(MAIN_OBJ) $(LDFLAGS) $(LIBS)

# --- 라이브러리 빌드 규칙 ---
$(LIB_DIR)/libcalc.a: $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(LIB_DIR)/libcalc.so: $(LIB_OBJS)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

# --- 목적 파일(.o) 생성 규칙 ---
# src/ 디렉토리의 .c 파일들을 obj/ 디렉토리에 .o 파일로 생성
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

# 루트 디렉토리의 .c 파일을 .o 파일로 생성 (main.c 용)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# --- 프로젝트 정리 규칙 ---
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TARGET_STATIC) $(TARGET_SHARED) $(MAIN_OBJ)
