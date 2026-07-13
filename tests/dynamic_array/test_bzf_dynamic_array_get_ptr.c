#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>

static void test_dynamic_array_get_ptr(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val = 42;
    bzf_dynamic_array_push_back(arr, &val);

    void *ptr = NULL;
    bzf_dynamic_array_get_ptr(arr, 0, &ptr);
    assert_non_null(ptr);
    assert_int_equal(*(int *)ptr, 42);

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_get_ptr),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
