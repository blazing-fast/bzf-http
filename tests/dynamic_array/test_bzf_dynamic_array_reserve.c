#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>

static void test_dynamic_array_reserve(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    assert_int_equal(bzf_dynamic_array_reserve(arr, 10), BZF_DYNAMIC_ARRAY_RESERVE_OK);
    assert_int_equal(arr->capacity, 10);

    assert_int_equal(bzf_dynamic_array_reserve(arr, 5), BZF_DYNAMIC_ARRAY_RESERVE_OK);

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_reserve),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
