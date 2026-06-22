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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bytes_tokenize_with_remainder),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
