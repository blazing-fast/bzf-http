#include "bzf_os.h"
#include <sys/socket.h>

int bzf_socket(const int domain, const int type, const int protocol) {
    return socket(domain, type, protocol);
}
