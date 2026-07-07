#include <stddef.h>
#include <stdlib.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static void* default_bzf_os_calloc(size_t nmemb, size_t size)
{
    return calloc(nmemb, size);
}

void* (*mock_bzf_os_calloc_fn)(size_t, size_t) = default_bzf_os_calloc;

void mock_bzf_os_calloc_set(void* (*fn)(size_t, size_t))
{
    mock_bzf_os_calloc_fn = fn ? fn : default_bzf_os_calloc;
}

void *bzf_os_calloc(size_t nmemb, size_t size)
{
    return mock_bzf_os_calloc_fn(nmemb, size);
}
