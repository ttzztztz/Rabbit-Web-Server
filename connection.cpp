#include "connection.h"

connection::connection() {
    this->read_ptr = 0;
    this->write_ptr = 0;
    this->req = nullptr;
    this->header_size = 0;
    this->body_size = 0;
}