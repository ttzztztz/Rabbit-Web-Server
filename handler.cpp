#include "handler.h"
#include "request.h"
#include "helper.h"
#include "response.h"

using std::cin, std::cout, std::cerr, std::endl;
const unsigned int MAX_BUF = 8192;

void handler::handle(int fd) {
    char buffer[MAX_BUF];

    request http_request{};
    helper::read_http_first_line(fd, http_request);
    helper::parse_header(fd, http_request);

    printf("Request parsed [%s] %s \n", http_request.method.c_str(), http_request.path.c_str());
    // todo: recv request body

    response http_response{};
    if (http_request.method != "GET") {
        http_response.code = "405";
        http_response.message = "Method not allowed";

        string build_response = http_response.build();
        write(fd, build_response.c_str(), build_response.size());
        return;
    }

    const string file_path = "./static" + http_request.path;
    optional<string> file_extension = helper::read_file_extension(http_request.path);
    struct stat file_stat{};
    FILE *fp = fopen(file_path.c_str(), "r");

    if (file_extension == std::nullopt
        || !helper::file_exist(file_path, file_stat)
        || fp == nullptr) {
        http_response.code = "404";
        http_response.message = "Not found";

        string build_response = http_response.build();
        write(fd, build_response.c_str(), build_response.size());
        return;
    }

    http_response.code = "200";
    http_response.message = "OK";
    http_response.header["Content-type"] = helper::get_file_type(file_extension.value());
    http_response.header["Content-length"] = std::to_string(file_stat.st_size);

    string build_response = http_response.build();
    write(fd, build_response.c_str(), build_response.size());

    int file_block_size = 0;
    while ((file_block_size = fread(buffer, sizeof(char), MAX_BUF, fp)) > 0) {
        write(fd, buffer, file_block_size);
    }
}

void handler::handle_print_client_info(sockaddr* client_addr, socklen_t client_addr_len) {
    char host_name[MAX_BUF], host_port[MAX_BUF];

    memset(host_name, 0, sizeof(host_name));
    memset(host_port, 0, sizeof(host_port));

    getnameinfo(client_addr, client_addr_len, host_name, sizeof(host_name),
                host_port, sizeof(host_port), 0);

    printf("Received Request from: %s, port: %s \n", host_name, host_port);
}