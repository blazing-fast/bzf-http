#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "bzf_http_server.h"
#include "bzf_os_mocks.h"
#include <errno.h>

static int per_test_setup(void **state)
{
    (void)state;
    mock_bzf_send_set(NULL);
    errno = 0;
    return 0;
}

static ssize_t fake_send_all(int fd, const void* buf, size_t n, int flags)
{
    (void)fd;
    (void)buf;
    (void)flags;
    return (ssize_t)n;
}

static int partial_call_count = 0;

static ssize_t fake_send_partial(int fd, const void* buf, size_t n, int flags)
{
    (void)fd;
    (void)buf;
    (void)flags;
    partial_call_count++;
    if (partial_call_count == 1) return 3;
    if (partial_call_count == 2) return 2;
    return (ssize_t)n;
}

static ssize_t fake_send_zero(int fd, const void* buf, size_t n, int flags)
{
    (void)fd;
    (void)buf;
    (void)flags;
    (void)n;
    return 0;
}

static ssize_t fake_send_error(int fd, const void* buf, size_t n, int flags)
{
    (void)fd;
    (void)buf;
    (void)flags;
    (void)n;
    return -1;
}

static void test_send_response_ok(void **state)
{
    (void)state;
    mock_bzf_send_set(fake_send_all);
    struct bzf_http_client client = {.file_descriptor = 1};
    bzf_byte_t data[] = "hello";
    struct bzf_bytes_immutable_view view = {data, 5};

    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_OK);
    mock_bzf_send_set(NULL);
}

static void test_send_response_partial(void **state)
{
    (void)state;
    partial_call_count = 0;
    mock_bzf_send_set(fake_send_partial);
    struct bzf_http_client client = {.file_descriptor = 1};
    bzf_byte_t data[] = "hello";
    struct bzf_bytes_immutable_view view = {data, 5};

    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_OK);
    mock_bzf_send_set(NULL);
}

static void test_send_response_closed_zero(void **state)
{
    (void)state;
    mock_bzf_send_set(fake_send_zero);
    struct bzf_http_client client = {.file_descriptor = 1};
    bzf_byte_t data[] = "hello";
    struct bzf_bytes_immutable_view view = {data, 5};

    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_CLOSED);
    mock_bzf_send_set(NULL);
}

static int eintr_call_count = 0;
static ssize_t fake_send_eintr(int fd, const void* buf, size_t n, int flags)
{
    (void)fd;
    (void)buf;
    (void)flags;
    (void)n;
    eintr_call_count++;
    if (eintr_call_count == 1) {
        errno = EINTR;
        return -1;
    }
    return 5;
}

static void test_send_response_eintr_retry(void **state)
{
    (void)state;
    eintr_call_count = 0;
    mock_bzf_send_set(fake_send_eintr);
    struct bzf_http_client client = {.file_descriptor = 1};
    bzf_byte_t data[] = "hello";
    struct bzf_bytes_immutable_view view = {data, 5};

    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_OK);
    mock_bzf_send_set(NULL);
}

#define DEFINE_CLOSED_TEST(name, errcode) \
static void test_send_response_##name(void **state) \
{ \
    (void)state; \
    errno = errcode; \
    mock_bzf_send_set(fake_send_error); \
    struct bzf_http_client client = {.file_descriptor = 1}; \
    bzf_byte_t data[] = "hello"; \
    struct bzf_bytes_immutable_view view = {data, 5}; \
    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_CLOSED); \
    mock_bzf_send_set(NULL); \
}

DEFINE_CLOSED_TEST(epipe, EPIPE)
DEFINE_CLOSED_TEST(econnreset, ECONNRESET)
DEFINE_CLOSED_TEST(etimedout, ETIMEDOUT)

#define DEFINE_ERROR_TEST(name, errcode) \
static void test_send_response_##name(void **state) \
{ \
    (void)state; \
    errno = errcode; \
    mock_bzf_send_set(fake_send_error); \
    struct bzf_http_client client = {.file_descriptor = 1}; \
    bzf_byte_t data[] = "hello"; \
    struct bzf_bytes_immutable_view view = {data, 5}; \
    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_ERROR); \
    mock_bzf_send_set(NULL); \
}

DEFINE_ERROR_TEST(ebadf, EBADF)
DEFINE_ERROR_TEST(enotsock, ENOTSOCK)
DEFINE_ERROR_TEST(enotconn, ENOTCONN)
DEFINE_ERROR_TEST(einval, EINVAL)
DEFINE_ERROR_TEST(eopnotsupp, EOPNOTSUPP)

static void test_send_response_generic_error(void **state)
{
    (void)state;
    errno = EIO;
    mock_bzf_send_set(fake_send_error);
    struct bzf_http_client client = {.file_descriptor = 1};
    bzf_byte_t data[] = "hello";
    struct bzf_bytes_immutable_view view = {data, 5};
    assert_int_equal(bzf_http_send_response(client, view), BZF_HTTP_SEND_RESPONSE_ERROR);
    mock_bzf_send_set(NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_send_response_ok, per_test_setup),
        cmocka_unit_test_setup(test_send_response_partial, per_test_setup),
        cmocka_unit_test_setup(test_send_response_closed_zero, per_test_setup),
        cmocka_unit_test_setup(test_send_response_eintr_retry, per_test_setup),
        cmocka_unit_test_setup(test_send_response_epipe, per_test_setup),
        cmocka_unit_test_setup(test_send_response_econnreset, per_test_setup),
        cmocka_unit_test_setup(test_send_response_etimedout, per_test_setup),
        cmocka_unit_test_setup(test_send_response_ebadf, per_test_setup),
        cmocka_unit_test_setup(test_send_response_enotsock, per_test_setup),
        cmocka_unit_test_setup(test_send_response_enotconn, per_test_setup),
        cmocka_unit_test_setup(test_send_response_einval, per_test_setup),
        cmocka_unit_test_setup(test_send_response_eopnotsupp, per_test_setup),
        cmocka_unit_test_setup(test_send_response_generic_error, per_test_setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
