#include <sys/types.h>
#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static ssize_t default_bzf_send(int fd, const void* buf, size_t n, int flags)
{
    return send(fd, buf, n, flags);
}

ssize_t (*mock_bzf_send_fn)(int, const void*, size_t, int) = default_bzf_send;

void mock_bzf_send_set(ssize_t (*fn)(int, const void*, size_t, int))
{
    mock_bzf_send_fn = fn ? fn : default_bzf_send;
}

ssize_t bzf_send(int fd, const void* buf, size_t n, int flags)
{
    return mock_bzf_send_fn(fd, buf, n, flags);
}
