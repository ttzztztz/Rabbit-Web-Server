#ifndef WEB_SERVER_REQUEST_H
#define WEB_SERVER_REQUEST_H

#include <string>
#include <unordered_map>

using std::string, std::unordered_map;

class request {
public:
    string method, path, version;
    unordered_map<string, string> header;
    string body;
};


#endif //WEB_SERVER_REQUEST_H
