#ifndef WEB_SERVER_RESPONSE_H
#define WEB_SERVER_RESPONSE_H

#include <string>
#include <unordered_map>

using std::string, std::unordered_map, std::pair;

class response {
public:
    string version, code, message;
    unordered_map<string, string> header;
    string build();
    response();
};


#endif //WEB_SERVER_RESPONSE_H
