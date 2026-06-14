#include <stdlib.h>

#include "bzf_os.h"

void *bzf_os_malloc(const size_t size)
{
    return malloc(size);
}