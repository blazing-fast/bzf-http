#include "bzf_os.h"
#include <sys/types.h>
#include <sys/socket.h>

ssize_t bzf_send(const int fd, const void *const buf, const size_t n, const int flags) {
    return send(fd, buf, n, flags);
}
