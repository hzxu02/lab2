#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dbus/dbus.h>

int main(int argc, char *argv[]) {
    DBusConnection *conn;
    DBusError err;
    DBusMessage *msg;
    DBusMessageIter args;
    char *msg_content = "Hello from Client!"; // 기본 메시지

    if (argc > 1) {
        msg_content = argv[1];
    }

    printf("=== DBus Client Started ===\n");

    // 1. 에러 초기화
    dbus_error_init(&err);

    // 2. 세션 버스에 연결
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Connection Error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    // 3. 메소드 호출 메시지 생성
    // 대상: test.method.server, 객체경로: /test/method/Object, 인터페이스: test.method.Type, 메소드: Notify
    msg = dbus_message_new_method_call("test.method.server", 
                                       "/test/method/Object", 
                                       "test.method.Type", 
                                       "Notify");
    if (NULL == msg) {
        fprintf(stderr, "Message Null\n");
        return 1;
    }

    // 4. 인자 추가 (문자열)
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &msg_content)) {
        fprintf(stderr, "Out Of Memory!\n");
        return 1;
    }

    // 5. 메시지 전송 (비동기 전송 후 Flush)
    if (!dbus_connection_send(conn, msg, NULL)) {
        fprintf(stderr, "Out Of Memory!\n");
        return 1;
    }
    dbus_connection_flush(conn);

    printf("[Client] 메시지 전송 완료: \"%s\"\n", msg_content);

    // 메시지 메모리 해제
    dbus_message_unref(msg);

    return 0;
}
