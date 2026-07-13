#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_bytes.h"

static void test_bytes_tokenize_basic(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello=world=test";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, 1};

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

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.length, 1);

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.length, 1);

    assert_false(bzf_bytes_tokenize(v, d, &offset, &range));
}

static void test_bytes_tokenize_delimiter_not_found(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_false(bzf_bytes_tokenize(v, d, &offset, &range));
}

static void test_bytes_tokenize_delimiter_at_start(void **state) {
    (void)state;
    bzf_byte_t buf[] = "=hello";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 0);
    assert_int_equal(offset, 1);
}

static void test_bytes_tokenize_delimiter_at_end(void **state) {
    (void)state;
    bzf_byte_t buf[] = "hello=";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.offset, 0);
    assert_int_equal(range.length, 5);
    assert_int_equal(offset, 6);

    assert_false(bzf_bytes_tokenize(v, d, &offset, &range));
}

static void test_bytes_tokenize_consecutive_delimiters(void **state) {
    (void)state;
    bzf_byte_t buf[] = "a==b";
    bzf_byte_t delim[] = "=";

    const struct bzf_bytes_immutable_view v = {buf, sizeof(buf) - 1};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.length, 1);
    assert_int_equal(offset, 2);

    assert_true(bzf_bytes_tokenize(v, d, &offset, &range));
    assert_int_equal(range.length, 0);
    assert_int_equal(offset, 3);

    assert_false(bzf_bytes_tokenize(v, d, &offset, &range));
}

static void test_bytes_tokenize_empty_buffer(void **state) {
    (void)state;
    bzf_byte_t buf[] = "";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 0};
    const struct bzf_bytes_immutable_view d = {delim, sizeof(delim) - 1};

    size_t offset = 0;
    struct bzf_bytes_range range;

    assert_false(bzf_bytes_tokenize(v, d, &offset, &range));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_tokenize_basic),
        cmocka_unit_test(test_bytes_tokenize_multiple),
        cmocka_unit_test(test_bytes_tokenize_delimiter_not_found),
        cmocka_unit_test(test_bytes_tokenize_delimiter_at_start),
        cmocka_unit_test(test_bytes_tokenize_delimiter_at_end),
        cmocka_unit_test(test_bytes_tokenize_consecutive_delimiters),
        cmocka_unit_test(test_bytes_tokenize_empty_buffer),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
