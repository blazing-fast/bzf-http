#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server_impl.h"
#include "bzf_hashmap_mocks.h"
#include <bzf_os_mocks.h>

static void test_http_add_route_handler_ok(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    assert_non_null(server);

    bzf_byte_t route_path[] = "/test";
    struct bzf_http_route_handler handler = {
        .route = {route_path, sizeof(route_path) - 1},
        .handler = NULL,
    };

    assert_int_equal(
        bzf_http_add_route_handler(server, handler),
        BZF_HTTP_ADD_ROUTE_HANDLER_OK
    );

    bzf_http_server_destroy(server);
}

static enum bzf_hashmap_insert_result fake_insert_always_fails(
    struct bzf_hashmap* const hashmap,
    const struct bzf_bytes_immutable_view key,
    void* value)
{
    (void)hashmap;
    (void)key;
    (void)value;
    return BZF_HASHMAP_INSERT_ALLOCATION_ERROR;
}

static void test_http_add_route_handler_hashmap_fails(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    mock_bzf_hashmap_insert_set(fake_insert_always_fails);

    bzf_byte_t route_path[] = "/test";
    struct bzf_http_route_handler handler = {
        .route = {route_path, sizeof(route_path) - 1},
        .handler = NULL,
    };

    assert_int_equal(
        bzf_http_add_route_handler(server, handler),
        BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR
    );

    mock_bzf_hashmap_insert_set(NULL);

    bzf_http_server_destroy(server);
}

static void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static void test_http_add_route_handler_malloc_fails(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    mock_bzf_os_malloc_set(malloc_always_fail);

    bzf_byte_t route_path[] = "/test";
    const struct bzf_http_route_handler handler = {
        .route = {route_path, sizeof(route_path) - 1},
        .handler = NULL,
    };

    assert_int_equal(
        bzf_http_add_route_handler(server, handler),
        BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR
    );

    mock_bzf_os_malloc_set(NULL);

    bzf_http_server_destroy(server);
}

static void test_http_add_route_handler_and_real_get(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    mock_bzf_hashmap_insert_set(NULL);

    bzf_byte_t route_path[] = "/real";
    struct bzf_http_route_handler handler = {
        .route = {route_path, sizeof(route_path) - 1},
        .handler = NULL,
    };
    assert_int_equal(bzf_http_add_route_handler(server, handler), BZF_HTTP_ADD_ROUTE_HANDLER_OK);

    void *found = NULL;
    enum bzf_hashmap_get_result get_res = bzf_hashmap_get(
        server->handlers.configured_routes, handler.route, &found);
    assert_int_equal(get_res, BZF_HASHMAP_GET_FOUND);
    assert_non_null(found);
    assert_null(((struct bzf_http_route_handler*)found)->handler);

    bzf_http_server_destroy(server);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_http_add_route_handler_ok),
        cmocka_unit_test(test_http_add_route_handler_hashmap_fails),
        cmocka_unit_test(test_http_add_route_handler_malloc_fails),
        cmocka_unit_test(test_http_add_route_handler_and_real_get),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
