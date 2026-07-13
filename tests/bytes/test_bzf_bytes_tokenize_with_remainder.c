#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_bytes.h"

static void test_bytes_tokenize_with_remainder(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello,world";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 11};
    const struct bzf_bytes_immutable_view d = {delim, 1};
    struct bzf_bytes_immutable_view token;

    size_t offset = 0;

    assert_true(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
    assert_int_equal(token.length, 5);

    assert_true(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
    assert_int_equal(token.length, 5);

    assert_false(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
}

static void test_bytes_tokenize_with_remainder_no_delimiter(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 5};
    const struct bzf_bytes_immutable_view d = {delim, 1};
    struct bzf_bytes_immutable_view token;

    size_t offset = 0;

    assert_true(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
    assert_int_equal(token.length, 5);
    assert_ptr_equal(token.buffer, buf);
    assert_int_equal(offset, 5);

    assert_false(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
}

static void test_bytes_tokenize_with_remainder_offset_at_end(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello,world";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 11};
    const struct bzf_bytes_immutable_view d = {delim, 1};
    struct bzf_bytes_immutable_view token;

    size_t offset = 11;

    assert_false(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
}

static void test_bytes_tokenize_with_remainder_empty_input(void **state) {
    (void)state;
    bzf_byte_t buf[] = "";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 0};
    const struct bzf_bytes_immutable_view d = {delim, 1};
    struct bzf_bytes_immutable_view token;

    size_t offset = 0;

    assert_false(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
}

static void test_bytes_tokenize_with_remainder_delimiter_at_start(void **state) {
    (void)state;
    bzf_byte_t buf[] = ",hello";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 6};
    const struct bzf_bytes_immutable_view d = {delim, 1};
    struct bzf_bytes_immutable_view token;

    size_t offset = 0;

    assert_true(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
    assert_int_equal(token.length, 0);
    assert_ptr_equal(token.buffer, buf);
    assert_int_equal(offset, 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_tokenize_with_remainder),
        cmocka_unit_test(test_bytes_tokenize_with_remainder_no_delimiter),
        cmocka_unit_test(test_bytes_tokenize_with_remainder_offset_at_end),
        cmocka_unit_test(test_bytes_tokenize_with_remainder_empty_input),
        cmocka_unit_test(test_bytes_tokenize_with_remainder_delimiter_at_start),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
