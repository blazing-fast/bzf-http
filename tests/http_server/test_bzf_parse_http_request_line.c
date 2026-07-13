#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "http.h"
#include "bzf_bytes.h"

static void test_parse_ok(void **state)
{
    (void)state;
    bzf_byte_t buf[] = "GET /index.html HTTP/1.1";
    struct bzf_bytes_immutable_view v = {buf, sizeof(buf)-1};
    struct http_request_line out;
    assert_true(parse_http_request_line(v, &out));
    assert_int_equal(out.method.offset, 0);
    assert_int_equal(out.method.length, 3);
    assert_int_equal(out.target.offset, 4);
    assert_int_equal(out.target.length, 11);
    assert_int_equal(out.version.offset, 16);
    assert_int_equal(out.version.length, 8);
}

static void test_parse_too_few_parts(void **state)
{
    (void)state;
    bzf_byte_t buf[] = "GET /index.html";
    struct bzf_bytes_immutable_view v = {buf, sizeof(buf)-1};
    struct http_request_line out;
    assert_false(parse_http_request_line(v, &out));
}

static void test_parse_empty(void **state)
{
    (void)state;
    bzf_byte_t buf[] = "";
    struct bzf_bytes_immutable_view v = {buf, 0};
    struct http_request_line out;
    assert_false(parse_http_request_line(v, &out));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_ok),
        cmocka_unit_test(test_parse_too_few_parts),
        cmocka_unit_test(test_parse_empty),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
