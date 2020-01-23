#ifndef WEB_SERVER_HANDLER_H
#define WEB_SERVER_HANDLER_H

#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

class handler {
public:
    static void handle(int fd, const sockaddr *client_addr, socklen_t client_addr_len);
};


#endif //WEB_SERVER_HANDLER_H
