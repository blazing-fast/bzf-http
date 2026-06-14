#include "bzf_http_server.h"
#include <assert.h>
#include <stdlib.h>

enum bzf_http_add_route_handler_result bzf_http_add_route_handler(const struct bzf_http_server* http_server,
                                                               const struct bzf_http_route_handler route_handler)
{
    assert(http_server != NULL);
    struct bzf_http_route_handler* value = malloc(sizeof(struct bzf_http_route_handler));
    if (value == NULL)
    {
        return BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR;
    }

    *value = route_handler;
    const enum bzf_hashmap_insert_result hashmap_insert_result = bzf_hashmap_insert(
        http_server->handlers.configured_routes, route_handler.route, (void*)value);
    if (hashmap_insert_result == BZF_HASHMAP_INSERT_ALLOCATION_ERROR)
    {
        return BZF_HTTP_ADD_ROUTE_HANDLER_MEMORY_ALLOCATION_ERROR;
    }
    return BZF_HTTP_ADD_ROUTE_HANDLER_OK;
}
