#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static int default_bzf_socket(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

int (*mock_bzf_socket_fn)(int, int, int) = default_bzf_socket;

void mock_bzf_socket_set(int (*fn)(int, int, int))
{
    mock_bzf_socket_fn = fn ? fn : default_bzf_socket;
}

int bzf_socket(int domain, int type, int protocol)
{
    return mock_bzf_socket_fn(domain, type, protocol);
}
