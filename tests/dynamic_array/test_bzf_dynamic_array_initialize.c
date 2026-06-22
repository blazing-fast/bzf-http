#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_dynamic_array.h"
#include <bzf_dynamic_array_impl.h>

static void test_dynamic_array_initialize(void **state) {
    (void)state;
    struct bzf_dynamic_array *arr = NULL;

    enum bzf_dynamic_array_initialize_result res =
        bzf_dynamic_array_initialize(sizeof(int), 4, &arr);

    assert_int_equal(res, BZF_DYNAMIC_ARRAY_INITIALIZE_OK);
    assert_non_null(arr);
    assert_int_equal(arr->number_of_elements, 0);
    assert_int_equal(arr->capacity, 4);
    assert_int_equal(arr->element_size, sizeof(int));

    free(arr->buffer);
    free(arr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dynamic_array_initialize),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
