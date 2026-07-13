#include "bzf_http_server.h"
#include <signal.h>

static volatile sig_atomic_t bzf_keep_running = 1;

static void bzf_sigint_handler(int sig)
{
    (void)sig;
    bzf_keep_running = 0;
}

void bzf_http_server_run(struct bzf_http_server *server)
{
    bzf_keep_running = 1;

    struct sigaction sa = {0};
    struct sigaction old_sa_int = {0};
    struct sigaction old_sa_term = {0};

    sa.sa_handler = bzf_sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, &old_sa_int);
    sigaction(SIGTERM, &sa, &old_sa_term);

    while (bzf_keep_running) {
        const enum bzf_http_server_accept_and_handle_result res =
            bzf_http_server_accept_and_handle(server);

        if (res != BZF_HTTP_SERVER_ACCEPT_AND_HANDLE_OK && bzf_keep_running) {
            break;
        }
    }

    sigaction(SIGINT, &old_sa_int, NULL);
    sigaction(SIGTERM, &old_sa_term, NULL);
}
