#ifndef WEB_SERVER_HANDLER_H
#define WEB_SERVER_HANDLER_H

#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fstream>
#include <memory>

#include "connection.h"
#include "request.h"
#include "helper.h"
#include "response.h"
#include "epoll_helper.h"

using std::shared_ptr;

class handler {
public:
    static void print_client_info(sockaddr* client_addr, socklen_t client_addr_len);
    
    static void read(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
    static void write(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
private:
    static void parse_header(shared_ptr<connection> conn);
    static bool write_file(int epoll_fd, int conn_fd, shared_ptr<connection> conn);

    static bool _read(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
    static bool _write(int epoll_fd, int conn_fd, shared_ptr<connection> conn);
};


#endif //WEB_SERVER_HANDLER_H
