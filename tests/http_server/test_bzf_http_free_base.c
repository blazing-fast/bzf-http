#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "private.h"
#include "bzf_os_mocks.h"

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_os_free_set(NULL);
    return 0;
}

static int free_count = 0;

static void counting_free(void* ptr)
{
    free_count++;
    free(ptr);
}

static void test_free_base_calls_os_free(void **state)
{
    (void)state;
    free_count = 0;
    mock_bzf_os_free_set(counting_free);
    int *dummy = malloc(sizeof(int));
    *dummy = 42;
    bzf_http_free_base((struct bzf_bytes_immutable_view){NULL, 0}, dummy);
    assert_int_equal(free_count, 1);
    mock_bzf_os_free_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_free_base_calls_os_free, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
