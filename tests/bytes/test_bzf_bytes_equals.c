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

    const struct bzf_bytes_immutable_view v1 = {buf1, 4};
    const struct bzf_bytes_immutable_view v2 = {buf2, 4};

    assert_true(bzf_bytes_equals(v1, v2));
}

static void test_bytes_equals_different_content(void **state) {
    (void)state;
    bzf_byte_t buf1[] = {1, 2, 3, 4};
    bzf_byte_t buf2[] = {1, 2, 3, 5};

    const struct bzf_bytes_immutable_view v1 = {buf1, 4};
    const struct bzf_bytes_immutable_view v2 = {buf2, 4};

    assert_false(bzf_bytes_equals(v1, v2));
}

static void test_bytes_equals_different_length(void **state) {
    (void)state;
    bzf_byte_t buf1[] = {1, 2, 3};
    bzf_byte_t buf2[] = {1, 2, 3, 4};

    const struct bzf_bytes_immutable_view v1 = {buf1, 3};
    const struct bzf_bytes_immutable_view v2 = {buf2, 4};

    assert_false(bzf_bytes_equals(v1, v2));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_equals_identical),
        cmocka_unit_test(test_bytes_equals_different_content),
        cmocka_unit_test(test_bytes_equals_different_length),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
