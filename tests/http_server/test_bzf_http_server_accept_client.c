#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server.h"
#include "bzf_os_mocks.h"
#include <sys/socket.h>

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

static int fake_accept_ok(int fd, struct sockaddr* addr, socklen_t* addr_len)
{
    (void)fd;
    (void)addr;
    (void)addr_len;
    return 42;
}

static int fake_accept_fail(int fd, struct sockaddr* addr, socklen_t* addr_len)
{
    (void)fd;
    (void)addr;
    (void)addr_len;
    return -1;
}

static int fake_setsockopt_ok(int fd, int level, int optname, const void* optval, socklen_t optlen)
{
    (void)fd;
    (void)level;
    (void)optname;
    (void)optval;
    (void)optlen;
    return 0;
}

static int fake_setsockopt_fail(int fd, int level, int optname, const void* optval, socklen_t optlen)
{
    (void)fd;
    (void)level;
    (void)optname;
    (void)optval;
    (void)optlen;
    return -1;
}

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_socket_set(fake_socket_ok);
    mock_bzf_bind_set(fake_bind_ok);
    mock_bzf_listen_set(fake_listen_ok);
    mock_bzf_setsockopt_set(fake_setsockopt_ok);
    mock_bzf_accept_set(NULL);
    return 0;
}

static void test_accept_client_ok(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    mock_bzf_accept_set(fake_accept_ok);

    struct bzf_http_client client;
    struct sockaddr_in client_addr;
    enum bzf_http_server_accept_client_result res = bzf_http_server_accept_client(server, &client, &client_addr);
    assert_int_equal(res, BZF_HTTP_SERVER_ACCEPT_CLIENT_OK);
    assert_int_equal(client.file_descriptor, 42);

    bzf_http_server_destroy(server);
    mock_bzf_accept_set(NULL);
}

static void test_accept_client_accept_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    mock_bzf_accept_set(fake_accept_fail);

    struct bzf_http_client client;
    struct sockaddr_in client_addr;
    enum bzf_http_server_accept_client_result res = bzf_http_server_accept_client(server, &client, &client_addr);
    assert_int_equal(res, BZF_HTTP_SERVER_ACCEPT_ERROR_WHEN_ACCEPTING);
    assert_int_equal(client.file_descriptor, -1);

    bzf_http_server_destroy(server);
    mock_bzf_accept_set(NULL);
}

static void test_accept_client_setsockopt_fails(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    mock_bzf_accept_set(fake_accept_ok);
    mock_bzf_setsockopt_set(fake_setsockopt_fail);

    struct bzf_http_client client;
    struct sockaddr_in client_addr;
    enum bzf_http_server_accept_client_result res = bzf_http_server_accept_client(server, &client, &client_addr);
    assert_int_equal(res, BZF_HTTP_SERVER_ACCEPT_CLIENT_ERROR_WHEN_SETSOCKETOPT);
    assert_int_equal(client.file_descriptor, 42);

    bzf_http_server_destroy(server);
    mock_bzf_accept_set(NULL);
    mock_bzf_setsockopt_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_accept_client_ok, per_test_setup),
        cmocka_unit_test_setup(test_accept_client_accept_fails, per_test_setup),
        cmocka_unit_test_setup(test_accept_client_setsockopt_fails, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
