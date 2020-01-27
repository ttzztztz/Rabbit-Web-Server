#include "epoll_helper.h"

void epoll_helper::create_event(int epoll_fd, int fd, int state) {
    struct epoll_event event{};
    event.events = state;
    event.data.fd = fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
}

void epoll_helper::delete_event(int epoll_fd, int fd, int state) {
    struct epoll_event event{};
    event.events = state;
    event.data.fd = fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
}

void epoll_helper::modify_event(int epoll_fd, int fd, int state) {
    struct epoll_event event{};
    event.events = state;
    event.data.fd = fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
}