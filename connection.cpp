#include "connection.h"

connection::connection() {
    this->read_ptr = 0;
    this->write_ptr = 0;
    this->req = nullptr;
}

// void connection::dispose() {
//     this->read_ptr = 0;
//     this->write_ptr = 0;

//     this->read.clear();
//     this->write.clear();
//     this->req.reset();
// }