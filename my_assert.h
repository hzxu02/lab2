#include <stdio.h>
#include <stdlib.h>

// NDEBUG 매크로가 정의되어 있으면 my_assert를 아무것도 하지 않도록 정의
#ifdef NDEBUG
    #define my_assert(expr) ((void)0)
#else
    // NDEBUG가 정의되지 않았을 때만 my_assert를 활성화
    #define my_assert(expr) \
        if (!(expr)) { \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", \
                    #expr, __FILE__, __LINE__); \
            abort(); \
        }
#endif
