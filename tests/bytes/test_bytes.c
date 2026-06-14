#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_bytes.h"

static void test_bytes_equals_identical(void **state) {
    (void)state;
    bzf_byte_t buf1[] = {1, 2, 3, 4};
    bzf_byte_t buf2[] = {1, 2, 3, 4};

    struct bzf_bytes_immutable_view v1 = {buf1, 4};
    struct bzf_bytes_immutable_view v2 = {buf2, 4};

    assert_true(bzf_bytes_equals(v1, v2));
}

static void test_bytes_equals_different_content(void **state) {
    (void)state;
    bzf_byte_t buf1[] = {1, 2, 3, 4};
    bzf_byte_t buf2[] = {1, 2, 3, 5};

    struct bzf_bytes_immutable_view v1 = {buf1, 4};
    struct bzf_bytes_immutable_view v2 = {buf2, 4};

    assert_false(bzf_bytes_equals(v1, v2));
}

static void test_bytes_equals_different_length(void **state) {
    (void)state;
    bzf_byte_t buf1[] = {1, 2, 3};
    bzf_byte_t buf2[] = {1, 2, 3, 4};

    struct bzf_bytes_immutable_view v1 = {buf1, 3};
    struct bzf_bytes_immutable_view v2 = {buf2, 4};

    assert_false(bzf_bytes_equals(v1, v2));
}

static void test_bytes_add_offset(void **state) {
    (void)state;
    bzf_byte_t buf[] = {10, 20, 30, 40, 50};

    struct bzf_bytes_immutable_view v = {buf, 5};
    struct bzf_bytes_immutable_view shifted = bzf_bytes_add_offset(v, 2);

    assert_int_equal(shifted.length, 3);
    assert_int_equal(shifted.buffer[0], 30);
    assert_int_equal(shifted.buffer[1], 40);
    assert_int_equal(shifted.buffer[2], 50);
}

static void test_bytes_tokenize_basic(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello=world=test";
    bzf_byte_t delim[] = "=";

    struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    struct bzf_bytes_immutable_view d = {delim, 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 5);
    assert_int_equal(offset, 6);
}

static void test_bytes_tokenize_multiple(void **state) {
    (void)state;
    bzf_byte_t buf[] = "a,b,c";
    bzf_byte_t delim[] = ",";

    struct bzf_bytes_immutable_view v = {buf, 5};
    struct bzf_bytes_immutable_view d = {delim, 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.length, 1);

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.length, 1);

    assert_false(bzf_bytes_tokenize(v, d, &offset, &range));
}

static void test_bytes_split(void **state) {
    (void)state;
    bzf_byte_t buf[] = "foo,bar,baz";
    bzf_byte_t delim[] = ",";

    struct bzf_bytes_immutable_view v = {buf, 11};
    struct bzf_bytes_immutable_view d = {delim, 1};

    struct bzf_bytes_range ranges[3];
    size_t count = bzf_bytes_split(v, d, ranges, 3);

    assert_int_equal(count, 3);
    assert_int_equal(ranges[0].offset, 0);
    assert_int_equal(ranges[0].length, 3);
    assert_int_equal(ranges[1].offset, 4);
    assert_int_equal(ranges[1].length, 3);
    assert_int_equal(ranges[2].offset, 8);
    assert_int_equal(ranges[2].length, 3);
}

static void test_bytes_tokenize_with_remainder(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello,world";
    bzf_byte_t delim[] = ",";

    struct bzf_bytes_immutable_view v = {buf, 11};
    struct bzf_bytes_immutable_view d = {delim, 1};
    struct bzf_bytes_immutable_view token;

    size_t offset = 0;

    assert_true(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
    assert_int_equal(token.length, 5);

    assert_true(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
    assert_int_equal(token.length, 5);

    assert_false(bzf_bytes_tokenize_with_remainder(v, d, &offset, &token));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_equals_identical),
        cmocka_unit_test(test_bytes_equals_different_content),
        cmocka_unit_test(test_bytes_equals_different_length),
        cmocka_unit_test(test_bytes_add_offset),
        cmocka_unit_test(test_bytes_tokenize_basic),
        cmocka_unit_test(test_bytes_tokenize_multiple),
        cmocka_unit_test(test_bytes_split),
        cmocka_unit_test(test_bytes_tokenize_with_remainder),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
