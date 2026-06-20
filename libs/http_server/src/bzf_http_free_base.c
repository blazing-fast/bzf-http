#include <bzf_os.h>

#include "bzf_http_server.h"
#include <stdlib.h>

void bzf_http_free_base(struct bzf_bytes_immutable_view key, void* data)
{
    (void)key;
    bzf_os_free(data);
}
