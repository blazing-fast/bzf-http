#include <stdlib.h>

#include "bzf_os.h"

void *bzf_os_calloc(const size_t nmemb, const size_t size)
{
    return calloc(nmemb, size);
}
