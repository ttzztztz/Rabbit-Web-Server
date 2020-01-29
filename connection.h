#ifndef WEB_SERVER_CONNECTION_H
#define WEB_SERVER_CONNECTION_H

#include <iostream>
#include <cstring>
#include <memory>

#include "request.h"

using std::string, std::unique_ptr;

class connection {
public:
    string read;
    string write;

    size_t header_size;
    size_t body_size;

    size_t read_ptr;
    size_t write_ptr;

    unique_ptr<request> req;

    connection();
    // void dispose();
};


#endif //WEB_SERVER_CONNECTION_H
