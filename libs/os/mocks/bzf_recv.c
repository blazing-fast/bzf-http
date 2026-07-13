#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static ssize_t default_bzf_recv(int sockfd, void *buf, size_t len, int flags)
{
    return recv(sockfd, buf, len, flags);
}

ssize_t (*mock_bzf_recv_fn)(int, void*, size_t, int) = default_bzf_recv;

void mock_bzf_recv_set(ssize_t (*fn)(int, void*, size_t, int))
{
    mock_bzf_recv_fn = fn ? fn : default_bzf_recv;
}

ssize_t bzf_recv(int sockfd, void *buf, size_t len, int flags)
{
    return mock_bzf_recv_fn(sockfd, buf, len, flags);
}
