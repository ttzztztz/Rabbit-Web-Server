#ifndef WEB_SERVER_HELPER_H
#define WEB_SERVER_HELPER_H

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <optional>
#include <unordered_set>
#include <sys/stat.h>

#include "request.h"

using std::string, std::stringstream, std::optional, std::unordered_set;

class helper {
public:
    static string readline_from_fd(int fd);
    static void read_http_first_line(int fd, request& req);
    static void parse_header(int fd, request& req);
    static optional<string> read_file_extension(const string& url);
    static string get_file_type(const string& file_extensions);
    static bool file_exist(const string& file_name, struct stat& file_stat);
};


#endif //WEB_SERVER_HELPER_H
