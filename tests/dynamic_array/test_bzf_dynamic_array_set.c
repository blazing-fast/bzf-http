#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>

static void test_dynamic_array_set(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val = 1;
    bzf_dynamic_array_push_back(arr, &val);

    int updated = 999;
    bzf_dynamic_array_set(arr, 0, &updated);

    int read;
    bzf_dynamic_array_get(arr, 0, &read);
    assert_int_equal(read, 999);

    free(arr->buffer);
    free(arr);
}

static void test_dynamic_array_set_second_element(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;
    bzf_dynamic_array_initialize(sizeof(int), 2, &arr);

    int val1 = 1, val2 = 2;
    bzf_dynamic_array_push_back(arr, &val1);
    bzf_dynamic_array_push_back(arr, &val2);

    int updated = 888;
    bzf_dynamic_array_set(arr, 1, &updated);

    int read;
    bzf_dynamic_array_get(arr, 1, &read);
    assert_int_equal(read, 888);

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_set),
        cmocka_unit_test(test_dynamic_array_set_second_element),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
