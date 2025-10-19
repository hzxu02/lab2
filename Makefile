# 컴파일러 및 플래그 정의
CC = gcc
CFLAGS = -Wall -I./include
LDFLAGS = -L./lib

# 생성될 파일 및 디렉토리 이름 정의
TARGET_STATIC = main_static
TARGET_SHARED = main_shared
LIB_DIR = lib
OBJ_DIR = obj

# 소스 및 목적 파일 목록 자동 생성
SRC_FILES = $(wildcard src/*.c)
OBJ_FILES = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

.PHONY: all clean

all: $(TARGET_STATIC) $(TARGET_SHARED)

# --- 정적 라이브러리 및 실행 파일 빌드 규칙 ---
$(TARGET_STATIC): main.c $(LIB_DIR)/libcalc.a
	$(CC) -static -o $@ main.c $(LDFLAGS) -lcalc -I./include

# 수정된 부분: ar 명령 실행 전에 lib 디렉토리 생성
$(LIB_DIR)/libcalc.a: $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

# --- 공유 라이브러리 및 실행 파일 빌드 규칙 ---
$(TARGET_SHARED): main.c $(LIB_DIR)/libcalc.so
	$(CC) -o $@ main.c $(LDFLAGS) -lcalc -I./include

# 수정된 부분: gcc -shared 명령 실행 전에 lib 디렉토리 생성
$(LIB_DIR)/libcalc.so: $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

# --- 목적 파일(.o) 생성 규칙 ---
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

# --- 프로젝트 정리 규칙 ---
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(TARGET_STATIC) $(TARGET_SHARED)
