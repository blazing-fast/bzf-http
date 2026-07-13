#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "http.h"
#include "private.h"
#include "bzf_os_mocks.h"
#include <string.h>

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_recv_set(NULL);
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

static ssize_t fake_recv_closed(int fd, void* buf, size_t len, int flags)
{
    (void)fd;
    (void)buf;
    (void)len;
    (void)flags;
    return 0;
}

static ssize_t fake_recv_error(int fd, void* buf, size_t len, int flags)
{
    (void)fd;
    (void)buf;
    (void)len;
    (void)flags;
    return -1;
}

static void setup_recv(const char* data)
{
    recv_data = data;
    recv_len = strlen(data);
    recv_offset = 0;
    mock_bzf_recv_set(fake_recv);
}

static void test_fetch_and_parse_ok_single_recv(void **state)
{
    (void)state;
    setup_recv("GET / HTTP/1.1\r\nHost: localhost\r\n\r\n");

    bzf_byte_t buffer[20000];
    struct bzf_bytes_mutable_buffer http_head_buffer = {buffer, 0, 20000};
    struct http_fetch_and_parse_head_output output;
    bzf_hashmap_initialize(&output.headers);

    enum http_fetch_and_parse_head_result res = http_fetch_and_parse_head(1, &http_head_buffer, &output);
    assert_int_equal(res, HTTP_FETCH_AND_PARSE_HEAD_OK);
    assert_non_null(output.headers);

    bzf_hashmap_free(output.headers, &bzf_http_free_base);
    mock_bzf_recv_set(NULL);
}

static void test_fetch_and_parse_connection_closed(void **state)
{
    (void)state;
    mock_bzf_recv_set(fake_recv_closed);

    bzf_byte_t buffer[20000];
    struct bzf_bytes_mutable_buffer http_head_buffer = {buffer, 0, 20000};
    struct http_fetch_and_parse_head_output output;
    bzf_hashmap_initialize(&output.headers);

    enum http_fetch_and_parse_head_result res = http_fetch_and_parse_head(1, &http_head_buffer, &output);
    assert_int_equal(res, HTTP_FETCH_AND_PARSE_HEAD_CONNECTION_CLOSED);
    bzf_hashmap_free(output.headers, &bzf_http_free_base);
    mock_bzf_recv_set(NULL);
}

static void test_fetch_and_parse_unexpected_error(void **state)
{
    (void)state;
    mock_bzf_recv_set(fake_recv_error);

    bzf_byte_t buffer[20000];
    struct bzf_bytes_mutable_buffer http_head_buffer = {buffer, 0, 20000};
    struct http_fetch_and_parse_head_output output;
    bzf_hashmap_initialize(&output.headers);

    enum http_fetch_and_parse_head_result res = http_fetch_and_parse_head(1, &http_head_buffer, &output);
    assert_int_equal(res, HTTP_FETCH_AND_PARSE_HEAD_UNEXPECTED_ERROR);
    bzf_hashmap_free(output.headers, &bzf_http_free_base);
    mock_bzf_recv_set(NULL);
}

static void test_fetch_and_parse_invalid_request_line(void **state)
{
    (void)state;
    setup_recv("BADLINE\r\n\r\n");

    bzf_byte_t buffer[20000];
    struct bzf_bytes_mutable_buffer http_head_buffer = {buffer, 0, 20000};
    struct http_fetch_and_parse_head_output output;
    bzf_hashmap_initialize(&output.headers);

    enum http_fetch_and_parse_head_result res = http_fetch_and_parse_head(1, &http_head_buffer, &output);
    assert_int_equal(res, HTTP_FETCH_AND_PARSE_HEAD_INVALID_REQUEST_LINE);
    bzf_hashmap_free(output.headers, &bzf_http_free_base);
    mock_bzf_recv_set(NULL);
}

static void test_fetch_and_parse_invalid_header(void **state)
{
    (void)state;
    setup_recv("GET / HTTP/1.1\r\nBadHeaderWithoutColon\r\n\r\n");

    bzf_byte_t buffer[20000];
    struct bzf_bytes_mutable_buffer http_head_buffer = {buffer, 0, 20000};
    struct http_fetch_and_parse_head_output output;
    bzf_hashmap_initialize(&output.headers);

    enum http_fetch_and_parse_head_result res = http_fetch_and_parse_head(1, &http_head_buffer, &output);
    assert_int_equal(res, HTTP_FETCH_AND_PARSE_HEAD_INVALID_HEADER);
    bzf_hashmap_free(output.headers, &bzf_http_free_base);
    mock_bzf_recv_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_fetch_and_parse_ok_single_recv, per_test_setup),
        cmocka_unit_test_setup(test_fetch_and_parse_connection_closed, per_test_setup),
        cmocka_unit_test_setup(test_fetch_and_parse_unexpected_error, per_test_setup),
        cmocka_unit_test_setup(test_fetch_and_parse_invalid_request_line, per_test_setup),
        cmocka_unit_test_setup(test_fetch_and_parse_invalid_header, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
