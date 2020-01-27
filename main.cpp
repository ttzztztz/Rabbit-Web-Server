#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/signal.h>

#include "handler.h"
#include "epoll_helper.h"

const unsigned int MAX_EVENTS = 128;
const unsigned int FD_SIZE = 1024;

int main() {
    signal(SIGPIPE, SIG_IGN);

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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        const int active = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < active; i++) {
            const int fd = events[i].data.fd, ev = events[i].events;
            if (fd == listen_fd && (ev & EPOLLIN)) {
                sockaddr_in client_addr{};
                socklen_t client_addr_len = sizeof(client_addr);

                int conn_fd = accept(listen_fd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
                if (conn_fd != -1) {
                    printf("[%d] Connection Established\n", conn_fd);

                    handler::handle_print_client_info(reinterpret_cast<sockaddr *>(&client_addr), client_addr_len);
                    epoll_helper::create_event(epoll_fd, conn_fd, EPOLLIN);
                }
            } else if (ev & EPOLLIN) {
                const int conn_fd = events[i].data.fd;
                printf("[%d] Start reading process\n", conn_fd);
                handler::handle(conn_fd);

                printf("[%d] Write OK\n", conn_fd);
                epoll_helper::delete_event(epoll_fd, conn_fd, EPOLLIN);
                close(conn_fd);
            }
        }
    }
#pragma clang diagnostic pop
    return 0;
}
