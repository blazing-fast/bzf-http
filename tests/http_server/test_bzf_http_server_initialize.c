#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server.h"
#include "bzf_os_mocks.h"

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_os_malloc_set(NULL);
    mock_bzf_os_calloc_set(NULL);
    mock_bzf_os_free_set(NULL);
    mock_bzf_socket_set(NULL);
    mock_bzf_bind_set(NULL);
    mock_bzf_listen_set(NULL);
    return 0;
}

static void* malloc_always_fail(size_t size)
{
    (void)size;
    return NULL;
}

static int socket_always_return_negative(int domain, int type, int protocol)
{
    (void)domain;
    (void)type;
    (void)protocol;
    return -1;
}

static int bind_always_return_negative(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
    (void)sockfd;
    (void)addr;
    (void)addrlen;
    return -1;
}

static int listen_always_return_negative(int sockfd, int backlog)
{
    (void)sockfd;
    (void)backlog;
    return -1;
}

static void* calloc_always_fail(size_t nmemb, size_t size)
{
    (void)nmemb;
    (void)size;
    return NULL;
}

/* === OK path ================================================== */
static void test_http_server_initialize_ok(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;

    const enum bzf_http_server_initialize_result res = bzf_http_server_initialize(&server);
    assert_int_equal(res, BZF_HTTP_SERVER_INITIALIZE_OK);
    assert_non_null(server);

    bzf_http_server_destroy(server);
}

/* === bzf_os_malloc fails ====================================== */
static void test_http_server_initialize_malloc_fails(void **state) {
    (void)state;
    struct bzf_http_server *server = NULL;

    mock_bzf_os_malloc_set(malloc_always_fail);
    assert_int_equal(
        bzf_http_server_initialize(&server),
        BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR
    );
    assert_null(server);
}

/* === bzf_socket fails ========================================= */
static void test_http_server_initialize_socket_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    mock_bzf_socket_set(socket_always_return_negative);
    assert_int_equal(
        bzf_http_server_initialize(&server),
        BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR
    );
    assert_non_null(server);
    bzf_http_server_destroy(server);
}

/* === bzf_bind fails =========================================== */
static void test_http_server_initialize_bind_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    mock_bzf_bind_set(bind_always_return_negative);

    assert_int_equal(
        bzf_http_server_initialize(&server),
        BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR
    );
    assert_non_null(server);
    bzf_http_server_destroy(server);
}

/* === bzf_listen fails ========================================= */
static void test_http_server_initialize_listen_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    mock_bzf_listen_set(listen_always_return_negative);

    assert_int_equal(
        bzf_http_server_initialize(&server),
        BZF_HTTP_SERVER_INITIALIZE_SOCKET_INITIALISATION_ERROR
    );
    assert_non_null(server);
    bzf_http_server_destroy(server);
}

/* === bzf_hashmap_initialize fails (calloc inside) ============= */
static void test_http_server_initialize_hashmap_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    mock_bzf_os_calloc_set(calloc_always_fail);

    assert_int_equal(
        bzf_http_server_initialize(&server),
        BZF_HTTP_SERVER_INITIALIZE_MEMORY_ALLOCATION_ERROR
    );
    assert_non_null(server);
    bzf_http_server_destroy(server);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_http_server_initialize_ok, per_test_setup),
        cmocka_unit_test_setup(test_http_server_initialize_malloc_fails, per_test_setup),
        cmocka_unit_test_setup(test_http_server_initialize_socket_fails, per_test_setup),
        cmocka_unit_test_setup(test_http_server_initialize_bind_fails, per_test_setup),
        cmocka_unit_test_setup(test_http_server_initialize_listen_fails, per_test_setup),
        cmocka_unit_test_setup(test_http_server_initialize_hashmap_fails, per_test_setup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
