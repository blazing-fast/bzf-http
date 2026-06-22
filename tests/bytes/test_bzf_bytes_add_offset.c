#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_bytes.h"

static void test_bytes_add_offset(void **state) {
    (void)state;
    bzf_byte_t buf[] = {10, 20, 30, 40, 50};

    const struct bzf_bytes_immutable_view v = {buf, 5};
    const uint8_t offset_add = 2;
    const struct bzf_bytes_immutable_view shifted = bzf_bytes_add_offset(v, offset_add);

    assert_int_equal(shifted.length, sizeof(buf) / sizeof(buf[0]) - offset_add);
    assert_ptr_equal(shifted.buffer, buf + offset_add);
    assert_ptr_equal(shifted.buffer + 1, buf + offset_add + 1);
    assert_ptr_equal(shifted.buffer + 2, buf + offset_add + 2);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_add_offset),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
