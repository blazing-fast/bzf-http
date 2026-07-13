#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

static int free_count = 0;

static void counting_free(struct bzf_bytes_immutable_view key, void *val)
{
    (void)key;
    (void)val;
    free_count++;
}

static void test_hashmap_free_with_free_fn(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t k1[] = "a";
    bzf_byte_t k2[] = "b";
    int v1 = 1, v2 = 2;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 1}, &v1), BZF_HASHMAP_INSERT_OK);
    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k2, 1}, &v2), BZF_HASHMAP_INSERT_OK);

    free_count = 0;
    bzf_hashmap_free(map, &counting_free);
    assert_int_equal(free_count, 2);
}

static void test_hashmap_free_without_free_fn(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;
    assert_int_equal(bzf_hashmap_initialize(&map), BZF_HASHMAP_INITIALIZE_OK);

    bzf_byte_t k1[] = "a";
    int v1 = 1;

    assert_int_equal(bzf_hashmap_insert(map, (struct bzf_bytes_immutable_view){k1, 1}, &v1), BZF_HASHMAP_INSERT_OK);

    bzf_hashmap_free(map, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_free_with_free_fn),
        cmocka_unit_test(test_hashmap_free_without_free_fn),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
