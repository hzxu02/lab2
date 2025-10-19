#include <stdio.h>
#include <dlfcn.h> 

int main() {
	void *handle;
	int(*add)(int,int);
	char *error;

	// 공유 라이브러리를 동적으로 로드
    handle = dlopen("./libcalc.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    // 라이브러리에서 'add' 함수의 주소를 찾아 함수 포인터에 할당
    add = dlsym(handle, "add");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        return 1;
    }

    printf("--- 동적 로딩 테스트 ---\n");
    printf("10 + 5 = %d\n", (*add)(10, 5));

    // 로드한 라이브러리 해제
    dlclose(handle);
    return 0;
}
