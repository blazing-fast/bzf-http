#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

static void test_hashmap_hash_string_consistency(void **state) {
    (void)state;
    bzf_byte_t key1[] = "consistent";
    bzf_byte_t key2[] = "consistent";

    struct bzf_bytes_immutable_view v1 = {key1, 10};
    struct bzf_bytes_immutable_view v2 = {key2, 10};

    assert_int_equal(bzf_hashmap_hash_string(v1), bzf_hashmap_hash_string(v2));
}

static void test_hashmap_hash_string_different(void **state)
{
    (void)state;
    bzf_byte_t key1[] = "different";
    bzf_byte_t key2[] = "different2";

    struct bzf_bytes_immutable_view v1 = {key1, 9};
    struct bzf_bytes_immutable_view v2 = {key2, 10};

    assert_int_not_equal(bzf_hashmap_hash_string(v1), bzf_hashmap_hash_string(v2));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_hash_string_consistency),
        cmocka_unit_test(test_hashmap_hash_string_different)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
