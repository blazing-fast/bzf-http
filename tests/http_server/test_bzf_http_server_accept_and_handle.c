#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
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
    mock_bzf_accept_set(NULL);
    mock_bzf_setsockopt_set(NULL);
    mock_bzf_recv_set(NULL);
    mock_bzf_send_set(NULL);
    mock_bzf_close_set(NULL);
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

static const char* recv_data = NULL;
static size_t recv_len = 0;
static size_t recv_offset = 0;

static ssize_t fake_recv(int fd, void* buf, size_t len, int flags)
{
    (void)fd;
    (void)flags;
    if (recv_offset >= recv_len) return 0;
    size_t remaining = recv_len - recv_offset;
    size_t to_copy = len < remaining ? len : remaining;
    memcpy(buf, recv_data + recv_offset, to_copy);
    recv_offset += to_copy;
    return (ssize_t)to_copy;
}

static ssize_t fake_recv_error(int fd, void* buf, size_t len, int flags)
{
    (void)fd;
    (void)buf;
    (void)len;
    (void)flags;
    return -1;
}

static ssize_t fake_send_all(int fd, const void* buf, size_t n, int flags)
{
    (void)fd;
    (void)buf;
    (void)flags;
    return (ssize_t)n;
}

static void setup_recv(const char* data)
{
    recv_data = data;
    recv_len = strlen(data);
    recv_offset = 0;
    mock_bzf_recv_set(fake_recv);
}

static int close_count = 0;
static int counting_close(int fd)
{
    close_count++;
    (void)fd;
    return 0;
}

static void test_accept_and_handle_ok(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    mock_bzf_accept_set(fake_accept_ok);
    mock_bzf_setsockopt_set(fake_setsockopt_ok);
    mock_bzf_send_set(fake_send_all);
    close_count = 0;
    mock_bzf_close_set(counting_close);

    setup_recv("GET / HTTP/1.1\r\n\r\n");

    enum bzf_http_server_accept_and_handle_result res = bzf_http_server_accept_and_handle(server);
    assert_int_equal(res, BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_OK);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_accept_set(NULL);
    mock_bzf_setsockopt_set(NULL);
    mock_bzf_recv_set(NULL);
    mock_bzf_send_set(NULL);
    mock_bzf_close_set(NULL);
}

static void test_accept_and_handle_accept_error(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    mock_bzf_accept_set(fake_accept_fail);

    enum bzf_http_server_accept_and_handle_result res = bzf_http_server_accept_and_handle(server);
    assert_int_equal(res, BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_ACCEPT_ERROR);

    bzf_http_server_destroy(server);
    mock_bzf_accept_set(NULL);
}

static void test_accept_and_handle_request_error(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);
    mock_bzf_accept_set(fake_accept_ok);
    mock_bzf_setsockopt_set(fake_setsockopt_ok);
    mock_bzf_recv_set(fake_recv_error);

    enum bzf_http_server_accept_and_handle_result res = bzf_http_server_accept_and_handle(server);
    assert_int_equal(res, BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_REQUEST_ERROR);

    bzf_http_server_destroy(server);
    mock_bzf_accept_set(NULL);
    mock_bzf_setsockopt_set(NULL);
    mock_bzf_recv_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_accept_and_handle_ok, per_test_setup),
        cmocka_unit_test_setup(test_accept_and_handle_accept_error, per_test_setup),
        cmocka_unit_test_setup(test_accept_and_handle_request_error, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
