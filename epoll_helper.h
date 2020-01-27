#ifndef WEB_SERVER_EPOLL_HELPER_H
#define WEB_SERVER_EPOLL_HELPER_H

#include <sys/epoll.h>

class epoll_helper {
public:
    static void create_event(int epoll_fd, int fd, int state);
    static void delete_event(int epoll_fd, int fd, int state);
    static void modify_event(int epoll_fd, int fd, int state);
};


#endif //WEB_SERVER_EPOLL_HELPER_H
