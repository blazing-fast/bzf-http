#include <unistd.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static int default_bzf_close(int fd)
{
    return close(fd);
}

int (*mock_bzf_close_fn)(int) = default_bzf_close;

void mock_bzf_close_set(int (*fn)(int))
{
    mock_bzf_close_fn = fn;
}

int bzf_close(int fd)
{
    return mock_bzf_close_fn(fd);
}
