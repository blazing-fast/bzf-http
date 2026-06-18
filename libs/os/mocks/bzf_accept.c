#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static int default_bzf_accept(int fd, struct sockaddr* addr, socklen_t* addr_len)
{
    return accept(fd, addr, addr_len);
}

int (*mock_bzf_accept_fn)(int, struct sockaddr*, socklen_t*) = default_bzf_accept;

void mock_bzf_accept_set(int (*fn)(int, struct sockaddr*, socklen_t*))
{
    mock_bzf_accept_fn = fn;
}

int bzf_accept(int fd, struct sockaddr* addr, socklen_t* addr_len)
{
    return mock_bzf_accept_fn(fd, addr, addr_len);
}
