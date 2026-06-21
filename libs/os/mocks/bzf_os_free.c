#include <stddef.h>
#include <stdlib.h>

#include "bzf_os.h"
#include "bzf_os_mocks.h"

static void default_bzf_os_free(void* ptr)
{
    free(ptr);
}

void (*mock_bzf_os_free_fn)(void*) = default_bzf_os_free;

void mock_bzf_os_free_set(void (*fn)(void*))
{
    mock_bzf_os_free_fn = fn ? fn : default_bzf_os_free;
}

void bzf_os_free(void* ptr)
{
    mock_bzf_os_free_fn(ptr);
}
