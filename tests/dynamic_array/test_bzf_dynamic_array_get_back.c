#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>

static void test_dynamic_array_get_back(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val1 = 10, val2 = 20;
    bzf_dynamic_array_push_back(arr, &val1);
    bzf_dynamic_array_push_back(arr, &val2);

    int back;
    bzf_dynamic_array_get_back(arr, &back);
    assert_int_equal(back, 20);

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_get_back),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
