#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server.h"

static void test_http_server_initialize(void **state) {
    (void)state;
    struct bzf_http_server server;

    enum bzf_http_server_initialize_result res = bzf_http_server_initialize(&server);
    assert_int_equal(res, BZF_HTTP_SERVER_INITIALIZE_OK);

    bzf_http_server_destroy(&server);
}

static void test_http_server_add_route(void **state) {
    (void)state;
    struct bzf_http_server server;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    bzf_byte_t route_path[] = "/test";
    struct bzf_http_route_handler handler = {
        .route = {route_path, sizeof(route_path) - 1},
        .handler = NULL,
    };

    assert_int_equal(
        bzf_http_add_route_handler(&server, handler),
        BZF_HTTP_ADD_ROUTE_HANDLER_OK
    );

    bzf_http_server_destroy(&server);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_http_server_initialize),
        cmocka_unit_test(test_http_server_add_route),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
