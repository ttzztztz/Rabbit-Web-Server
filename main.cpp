#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unordered_map>
#include <csignal>
#include <memory>

#include "connection.h"
#include "helper.h"
#include "request.h"
#include "handler.h"
#include "epoll_helper.h"
#include "thread_poll.h"

const unsigned int MAX_EVENTS = 128;
const unsigned int FD_SIZE = 1024;

using std::bind, std::unordered_map, std::shared_ptr;
unordered_map<int, shared_ptr<connection>> connection_storage;

int main() {
    signal(SIGPIPE, SIG_IGN);

    thread_poll poll(8);
    
    sockaddr_in server_addr{};
    server_addr.sin_port = htons(8788);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int epoll_fd = epoll_create(FD_SIZE);
    struct epoll_event events[MAX_EVENTS];

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    epoll_helper::create_event(epoll_fd, listen_fd, EPOLLIN);

    bind(listen_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
    listen(listen_fd, 5);
    helper::set_non_block(listen_fd);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        const int active = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < active; i++) {
            const int fd = events[i].data.fd, ev = events[i].events;
            if (fd == listen_fd && (ev & (EPOLLIN | EPOLLERR))) {
                sockaddr_in client_addr{};
                socklen_t client_addr_len = sizeof(client_addr);

                int conn_fd = accept(listen_fd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
                if (conn_fd != -1) {
                    printf("[%d] Connection Established\n", conn_fd);

                    helper::set_non_block(conn_fd);
                    handler::print_client_info(reinterpret_cast<sockaddr *>(&client_addr), client_addr_len);
                    epoll_helper::create_event(epoll_fd, conn_fd, EPOLLIN | EPOLLOUT | EPOLLET);

                    if (connection_storage.count(conn_fd)) {
                        connection_storage.erase(conn_fd);
                    }
                    connection_storage[conn_fd] = unique_ptr<connection>(new connection());
                }
            } else if (ev & (EPOLLIN | EPOLLERR)) {
                int conn_fd = events[i].data.fd;
                printf("[%d] connect epoll_in event \n", conn_fd);
                if (!connection_storage.count(conn_fd)) {
                    printf("[%d] conn_fd invalid \n", conn_fd);
                    continue;
                }

                poll.push(bind(handler::read, epoll_fd, conn_fd, connection_storage[conn_fd]));
            } else if (ev & EPOLLOUT) {
                int conn_fd = events[i].data.fd;
                printf("[%d] connect epoll_out event \n", conn_fd);
                if (!connection_storage.count(conn_fd)) {
                    printf("[%d] conn_fd invalid \n", conn_fd);
                    continue;
                }

                poll.push(bind(handler::write, epoll_fd, conn_fd, connection_storage[conn_fd]));
            }
        }
    }
#pragma clang diagnostic pop
    return 0;
}
