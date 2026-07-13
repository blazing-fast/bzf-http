#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_bytes.h"

static void test_bytes_tokenize_once_found(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello=world";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize_once(v, d, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 5);
}

static void test_bytes_tokenize_once_not_found(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};
    struct bzf_bytes_range range;

    assert_false(bzf_bytes_tokenize_once(v, d, &range));
}

static void test_bytes_tokenize_once_delimiter_at_start(void **state) {
    (void)state;
    bzf_byte_t buf[] = "=hello";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize_once(v, d, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 0);
}

static void test_bytes_tokenize_once_delimiter_at_end(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello=";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize_once(v, d, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 5);
}

static void test_bytes_tokenize_once_long_delimiter(void **state) {
    (void)state;
    bzf_byte_t buf[] = "foo::bar";
    bzf_byte_t delim[] = "::";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize_once(v, d, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 3);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_tokenize_once_found),
        cmocka_unit_test(test_bytes_tokenize_once_not_found),
        cmocka_unit_test(test_bytes_tokenize_once_delimiter_at_start),
        cmocka_unit_test(test_bytes_tokenize_once_delimiter_at_end),
        cmocka_unit_test(test_bytes_tokenize_once_long_delimiter),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
