#include "bzf_http_server.h"
#include <stdio.h>

void printf_bytes_t(const struct bzf_bytes_immutable_view bytes)
{
    printf("%.*s\n", (int)bytes.length, (const char*)bytes.buffer);
}
