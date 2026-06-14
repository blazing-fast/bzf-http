#include "private.h"
#include <assert.h>
#include <unistd.h>

void bzf_http_server_destroy(struct bzf_http_server* http_server)
{
    assert(http_server != NULL);

    if (http_server->file_descriptor >= 0)
    {
        close(http_server->file_descriptor);
    }

    bzf_hashmap_free(http_server->handlers.configured_routes, &bzf_http_free_base);
}
