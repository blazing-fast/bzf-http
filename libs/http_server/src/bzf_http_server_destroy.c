#include "private.h"
#include "bzf_http_server_impl.h"
#include <assert.h>
#include <bzf_os.h>
#include <unistd.h>

void bzf_http_server_destroy(struct bzf_http_server* http_server) {
    assert(http_server != NULL);

    if (http_server->file_descriptor >= 0)
    {
        bzf_close(http_server->file_descriptor);
    }

    if (http_server->handlers.configured_routes != NULL)
    {
        bzf_hashmap_free(http_server->handlers.configured_routes, &bzf_http_free_base);
        http_server->handlers.configured_routes = NULL;
    }

    bzf_os_free(http_server);
}
