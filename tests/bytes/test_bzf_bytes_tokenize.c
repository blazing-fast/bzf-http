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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_tokenize_basic),
        cmocka_unit_test(test_bytes_tokenize_multiple),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
