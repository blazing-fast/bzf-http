#include <sys/socket.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static int default_bzf_listen(int fd, int backlog)
{
    return listen(fd, backlog);
}

int (*mock_bzf_listen_fn)(int, int) = default_bzf_listen;

void mock_bzf_listen_set(int (*fn)(int, int))
{
    mock_bzf_listen_fn = fn;
}

int bzf_listen(int fd, int backlog)
{
    return mock_bzf_listen_fn(fd, backlog);
}
