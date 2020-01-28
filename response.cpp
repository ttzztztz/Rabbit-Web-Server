#include "response.h"

string response::build() {
    string response_header = this->version + " " + this->code + " " + this->message + "\r\n";
    for (const pair<const string, string> &one_pair : this->header) {
        response_header += one_pair.first + ": " + one_pair.second + "\r\n";
    }
    response_header += "\r\n";
    return response_header;
}

response::response() {
    this->version = "HTTP/1.0";
    this->header["Connection"] = "close";
    this->header["Server"] = "Rabbit-Web-Server";
}
