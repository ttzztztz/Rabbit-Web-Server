#ifndef WEB_SERVER_HELPER_H
#define WEB_SERVER_HELPER_H

#include <iostream>
#include <unistd.h>
#include <sstream>
#include <optional>
#include <cstring>
#include <unordered_set>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory>

#include "request.h"
#include "connection.h"

using std::string, std::stringstream, std::optional, std::unordered_set, std::memset, std::shared_ptr;

class helper {
public:
    static string readline_from_connection(shared_ptr<connection> conn);
    static void parse_header(shared_ptr<connection> conn);
    static void parse_body(shared_ptr<connection> conn);
    static optional<string> read_file_extension(const string& url);
    static string get_file_type(const string& file_extensions);
    static bool file_exist(const string& file_name, struct stat& file_stat);
    static void set_non_block(const int fd);
private:
    static void read_http_first_line(shared_ptr<connection> conn);
    static int read_one_char_from_connection(char& buff, shared_ptr<connection> conn);
};


#endif //WEB_SERVER_HELPER_H
