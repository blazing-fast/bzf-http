#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_hashmap.h"

static void test_hashmap_initialize(void **state) {
    (void)state;
    struct bzf_hashmap *map = NULL;

    enum bzf_hashmap_initialize_result res = bzf_hashmap_initialize(&map);
    assert_int_equal(res, BZF_HASHMAP_INITIALIZE_OK);
    assert_non_null(map);

    bzf_hashmap_free(map, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_hashmap_initialize),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
