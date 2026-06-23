#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server.h"
#include "bzf_os_mocks.h"

static void test_http_server_initialize_ok(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;

    enum bzf_http_server_initialize_result res = bzf_http_server_initialize(&server);
    assert_int_equal(res, BZF_HTTP_SERVER_INITIALIZE_OK);
    assert_non_null(server);

    bzf_http_server_destroy(server);
}

static int socket_always_return_negative(int domain, int type, int protocol)
{
    (void)domain;
    (void)type;
    (void)protocol;
    return -1;
}

static void test_http_server_initialize_socket_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    mock_bzf_socket_set(socket_always_return_negative);
    assert_int_equal(
        bzf_http_server_initialize(&server),
        BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR
    );
    bzf_http_server_destroy(server);
    mock_bzf_socket_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_http_server_initialize_ok),
        cmocka_unit_test(test_http_server_initialize_socket_fails),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
