#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>
#include "private.h"
#include "bzf_http_server_impl.h"
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
    mock_bzf_recv_set(NULL);
    mock_bzf_send_set(NULL);
    mock_bzf_close_set(NULL);
    mock_bzf_os_malloc_set(NULL);
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

static void route_handler(struct bzf_http_route_output* output)
{
    static const unsigned char resp[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 2\r\n"
        "\r\n"
        "OK";
    const struct bzf_bytes_mutable_buffer buf = {
        (bzf_byte_t*)resp, sizeof(resp) - 1, sizeof(resp) - 1
    };
    *output->response = buf;
}

static void test_handle_request_ok_default(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    mock_bzf_send_set(fake_send_all);
    close_count = 0;
    mock_bzf_close_set(counting_close);
    setup_recv("GET / HTTP/1.1\r\n\r\n");

    struct bzf_http_client client = {.file_descriptor = 1};
    enum bzf_handle_request_result res = bzf_http_handle_request(client, server->handlers);
    assert_int_equal(res, BZF_HANDLE_REQUEST_OK);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_send_set(NULL);
    mock_bzf_close_set(NULL);
    mock_bzf_recv_set(NULL);
}

static void test_handle_request_ok_route(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    bzf_byte_t route_path[] = "/special";
    struct bzf_http_route_handler handler = {
        .route = {route_path, sizeof(route_path) - 1},
        .handler = &route_handler,
    };
    assert_int_equal(bzf_http_add_route_handler(server, handler), BZF_HTTP_ADD_ROUTE_HANDLER_OK);

    mock_bzf_send_set(fake_send_all);
    close_count = 0;
    mock_bzf_close_set(counting_close);
    setup_recv("GET /special HTTP/1.1\r\n\r\n");

    struct bzf_http_client client = {.file_descriptor = 1};
    enum bzf_handle_request_result res = bzf_http_handle_request(client, server->handlers);
    assert_int_equal(res, BZF_HANDLE_REQUEST_OK);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_send_set(NULL);
    mock_bzf_close_set(NULL);
    mock_bzf_recv_set(NULL);
}

static void test_handle_request_closed(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    close_count = 0;
    mock_bzf_close_set(counting_close);
    setup_recv("");

    struct bzf_http_client client = {.file_descriptor = 1};
    enum bzf_handle_request_result res = bzf_http_handle_request(client, server->handlers);
    assert_int_equal(res, BZF_HANDLE_REQUEST_CLOSED);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_close_set(NULL);
    mock_bzf_recv_set(NULL);
}

static void test_handle_request_invalid_request_line(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    close_count = 0;
    mock_bzf_close_set(counting_close);
    setup_recv("BADLINE\r\n\r\n");

    struct bzf_http_client client = {.file_descriptor = 1};
    enum bzf_handle_request_result res = bzf_http_handle_request(client, server->handlers);
    assert_int_equal(res, BZF_HANDLE_REQUEST_INVALID);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_close_set(NULL);
    mock_bzf_recv_set(NULL);
}

static void test_handle_request_unexpected_error(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    close_count = 0;
    mock_bzf_close_set(counting_close);
    mock_bzf_recv_set(fake_recv_error);

    struct bzf_http_client client = {.file_descriptor = 1};
    enum bzf_handle_request_result res = bzf_http_handle_request(client, server->handlers);
    assert_int_equal(res, BZF_HANDLE_REQUEST_UNEXPECTED_ERROR);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_close_set(NULL);
    mock_bzf_recv_set(NULL);
}

static int malloc_count = 0;
static void* counting_malloc_fail_second(size_t size)
{
    malloc_count++;
    if (malloc_count == 2) return NULL;
    return malloc(size);
}

static void test_handle_request_memory_allocation_error(void **state)
{
    (void)state;
    struct bzf_http_server *server = NULL;
    assert_int_equal(bzf_http_server_initialize(&server), BZF_HTTP_SERVER_INITIALIZE_OK);

    close_count = 0;
    mock_bzf_close_set(counting_close);
    setup_recv("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");
    malloc_count = 0;
    mock_bzf_os_malloc_set(counting_malloc_fail_second);

    struct bzf_http_client client = {.file_descriptor = 1};
    enum bzf_handle_request_result res = bzf_http_handle_request(client, server->handlers);
    assert_int_equal(res, BZF_HANDLE_REQUEST_MEMORY_ALLOCATION_ERROR);
    assert_int_equal(close_count, 1);

    bzf_http_server_destroy(server);
    mock_bzf_close_set(NULL);
    mock_bzf_recv_set(NULL);
    mock_bzf_os_malloc_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_handle_request_ok_default, per_test_setup),
        cmocka_unit_test_setup(test_handle_request_ok_route, per_test_setup),
        cmocka_unit_test_setup(test_handle_request_closed, per_test_setup),
        cmocka_unit_test_setup(test_handle_request_invalid_request_line, per_test_setup),
        cmocka_unit_test_setup(test_handle_request_unexpected_error, per_test_setup),
        cmocka_unit_test_setup(test_handle_request_memory_allocation_error, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
