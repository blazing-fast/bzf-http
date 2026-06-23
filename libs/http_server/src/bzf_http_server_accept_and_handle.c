#include "private.h"
#include "bzf_http_server_impl.h"

enum bzf_http_server_accept_and_handle_result bzf_http_server_accept_and_handle(struct bzf_http_server* server)
{
    struct bzf_http_client client;
    struct sockaddr_in client_addr;

    const enum bzf_http_server_accept_client_result accept_res =
        bzf_http_server_accept_client(server, &client, &client_addr);
    if (accept_res != BZF_HTTP_SERVER_ACCEPT_CLIENT_OK)
    {
        return BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_ACCEPT_ERROR;
    }

    const enum bzf_handle_request_result handle_res =
        bzf_http_handle_request(client, server->handlers);
    if (handle_res != BZF_HANDLE_REQUEST_OK)
    {
        return BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_REQUEST_ERROR;
    }

    return BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_OK;
}
