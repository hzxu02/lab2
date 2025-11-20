#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>

int main() {
    DBusConnection *conn;
    DBusError err;
    int ret;

    printf("=== DBus Server Started ===\n");

    // 1. 에러 초기화
    dbus_error_init(&err);

    // 2. 세션 버스에 연결
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // 3. Well-known Name 요청 (서버의 이름 등록)
    ret = dbus_bus_request_name(conn, "test.method.server", 
                                DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name Error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    printf("Listening on 'test.method.server'...\n");

    // 4. 메시지 루프
    while (1) {
        // 비블로킹 방식으로 읽기/쓰기 처리
        dbus_connection_read_write(conn, 0);
        
        // 메시지 꺼내기
        DBusMessage *msg = dbus_connection_pop_message(conn);

        if (NULL == msg) {
            usleep(100000); // 메시지가 없으면 잠시 대기 (CPU 점유율 방지)
            continue;
        }

        // 5. 메소드 호출 메시지인지 확인
        if (dbus_message_is_method_call(msg, "test.method.Type", "Notify")) {
            char *sigvalue;
            
            // 인자(문자열) 읽기
            if (dbus_message_get_args(msg, &err, DBUS_TYPE_STRING, &sigvalue, DBUS_TYPE_INVALID)) {
                printf("[Server] 알림 수신: %s\n", sigvalue);
            } else {
                fprintf(stderr, "Argument Error: %s\n", err.message);
                dbus_error_free(&err);
            }
        }

        // 메시지 메모리 해제
        dbus_message_unref(msg);
    }

    return 0;
}
