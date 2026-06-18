#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static int default_bzf_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen)
{
    return setsockopt(fd, level, optname, optval, optlen);
}

int (*mock_bzf_setsockopt_fn)(int, int, int, const void*, socklen_t) = default_bzf_setsockopt;

void mock_bzf_setsockopt_set(int (*fn)(int, int, int, const void*, socklen_t))
{
    mock_bzf_setsockopt_fn = fn;
}

int bzf_setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen)
{
    return mock_bzf_setsockopt_fn(fd, level, optname, optval, optlen);
}
