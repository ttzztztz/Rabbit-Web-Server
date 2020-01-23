#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

#include "handler.h"

int main() {
    sockaddr_in server_addr{};
    server_addr.sin_port = htons(8788);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listen_fd, reinterpret_cast<sockaddr *>(&server_addr), sizeof(server_addr));
    listen(listen_fd, 5);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        int conn_fd = accept(listen_fd, reinterpret_cast<sockaddr *>(&client_addr), &client_addr_len);
        handler::handle(conn_fd, reinterpret_cast<sockaddr *>(&client_addr), client_addr_len);
        close(conn_fd);
    }
#pragma clang diagnostic pop
    return 0;
}
