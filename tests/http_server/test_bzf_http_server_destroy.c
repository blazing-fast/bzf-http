#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server.h"
#include "bzf_os_mocks.h"

static int fake_socket_ok(int domain, int type, int protocol)
{
    (void)domain;
    (void)type;
    (void)protocol;
    return 42;
}

static int fake_bind_ok(int fd, const struct sockaddr* addr, socklen_t len)
{
    (void)fd;
    (void)addr;
    (void)len;
    return 0;
}

static int fake_listen_ok(int fd, int backlog)
{
    (void)fd;
    (void)backlog;
    return 0;
}

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_socket_set(fake_socket_ok);
    mock_bzf_bind_set(fake_bind_ok);
    mock_bzf_listen_set(fake_listen_ok);
    mock_bzf_close_set(NULL);
    mock_bzf_os_free_set(NULL);
    return 0;
}

static int close_call_count = 0;

static int counting_close(int fd)
{
    close_call_count++;
    (void)fd;
    return 0;
}

static int free_count = 0;

static void counting_free(void* ptr)
{
    free_count++;
    free(ptr);
}

static void test_http_server_destroy_closes_fd_and_frees(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    assert_non_null(server);

    close_call_count = 0;
    free_count = 0;
    mock_bzf_close_set(counting_close);
    mock_bzf_os_free_set(counting_free);

    bzf_http_server_destroy(server);

    assert_int_equal(close_call_count, 1);
    /* os_free is called for hashmap nodes, elements, hashmap struct, and server struct */
    assert_true(free_count >= 3);

    mock_bzf_close_set(NULL);
    mock_bzf_os_free_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_http_server_destroy_closes_fd_and_frees, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
