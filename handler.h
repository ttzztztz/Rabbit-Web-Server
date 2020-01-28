#ifndef WEB_SERVER_HANDLER_H
#define WEB_SERVER_HANDLER_H

#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <csignal>

class handler {
public:
    static void handle_print_client_info(sockaddr* client_addr, socklen_t client_addr_len);
    static void handle(int epoll_fd, int conn_fd);
private:
    static void _handle_read(int fd);
};


#endif //WEB_SERVER_HANDLER_H
