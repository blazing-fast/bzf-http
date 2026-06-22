#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_bytes.h"

static void test_bytes_split(void **state) {
    (void)state;
    bzf_byte_t buf[] = "foo,bar,baz";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 11};
    const struct bzf_bytes_immutable_view d = {delim, 1};

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

static void test_bytes_split_with_remainder_after_max_token(void** state) {
    (void)state;
    bzf_byte_t buf[] = "foo,bar,baz";
    bzf_byte_t delim[] = ",";

    const struct bzf_bytes_immutable_view v = {buf, 11};
    const struct bzf_bytes_immutable_view d = {delim, 1};

    struct bzf_bytes_range ranges[2];
    size_t count = bzf_bytes_split(v, d, ranges, 2);

    assert_int_equal(count, 2);
    assert_int_equal(ranges[0].offset, 0);
    assert_int_equal(ranges[0].length, 3);
    assert_int_equal(ranges[1].offset, 4);
    assert_int_equal(ranges[1].length, 7);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_split),
        cmocka_unit_test(test_bytes_split_with_remainder_after_max_token),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
