#include "helper.h"

string helper::readline_from_fd(int fd) {
    string result;
    char last_buf = '\0', buf = '\0';

    while (read(fd, &buf, 1) > 0) {
        if (last_buf == '\r' && buf == '\n') {
            result.pop_back();
            break;
        }

        if (buf == '\0') {
            break;
        }

        result += buf;
        last_buf = buf;
    }

    return result;
}

void helper::read_http_first_line(int fd, request &req) {
    stringstream ss;

    ss << readline_from_fd(fd);
    ss >> req.method >> req.path >> req.version;
}

void helper::parse_header(int fd, request &req) {
    string buffer;
    while (!(buffer = helper::readline_from_fd(fd)).empty()) {
        const size_t line_length = buffer.size();
        string key, value;
        size_t ptr = 0;
        while (ptr < line_length) {
            if (buffer[ptr] == ':') {
                break;
            }
            ptr++;
        }

        key = buffer.substr(0, ptr);
        value = buffer.substr(ptr + 2);

        req.header[key] = value;
    }
}

optional<string> helper::read_file_extension(const string &url) {
    int index = url.find('.');
    if (index == std::string::npos) {
        return std::nullopt;
    }

    string result;
    const size_t url_size = url.size();
    for (int i = 0; i < url_size; i++) {
        if (url[url_size - i - 1] == '.') break;
        result.insert(result.begin(), url[url_size - i - 1]);
    }
    return result;
}

string helper::get_file_type(const string &file_extensions) {
    const unordered_set<string> image_type_collection = {"jpg", "jpeg", "png", "gif", "bmp"};

    if (image_type_collection.count(file_extensions)) {
        return "image/" + file_extensions;
    } else {
        return "text/plain";
    }
}

bool helper::file_exist(const string &file_name, struct stat& file_stat) {
    return stat(file_name.c_str(), &file_stat) >= 0;
}
