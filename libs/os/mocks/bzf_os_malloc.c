#include <stddef.h>
#include <stdlib.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static void* default_bzf_os_malloc(size_t size)
{
    return malloc(size);
}

void* (*mock_bzf_os_malloc_fn)(size_t) = default_bzf_os_malloc;

void mock_bzf_os_malloc_set(void* (*fn)(size_t))
{
    mock_bzf_os_malloc_fn = fn;
}

void *bzf_os_malloc(size_t size)
{
    return mock_bzf_os_malloc_fn(size);
}
