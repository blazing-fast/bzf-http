#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static int default_bzf_bind(int fd, const struct sockaddr* addr, socklen_t len)
{
    return bind(fd, addr, len);
}

int (*mock_bzf_bind_fn)(int, const struct sockaddr*, socklen_t) = default_bzf_bind;

void mock_bzf_bind_set(int (*fn)(int, const struct sockaddr*, socklen_t))
{
    mock_bzf_bind_fn = fn ? fn : default_bzf_bind;
}

int bzf_bind(int fd, const struct sockaddr* addr, socklen_t len)
{
    return mock_bzf_bind_fn(fd, addr, len);
}
