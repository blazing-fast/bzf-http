#include "bzf_os.h"
#include <sys/socket.h>

int bzf_bind(const int fd, const struct sockaddr *const addr, const socklen_t len) {
    return bind(fd, addr, len);
}
