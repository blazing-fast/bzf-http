#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "private.h"
#include "bzf_bytes.h"

static void test_default_handler_sets_response(void **state)
{
    (void)state;
    struct bzf_bytes_mutable_buffer buf = {0};
    struct bzf_http_route_output out = {&buf};
    defaultHandler(&out);
    assert_int_equal(buf.length, sizeof(
        "HTTP/1.1 404 NOT FOUND\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 9\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Not found"
    ) - 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_default_handler_sets_response),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
