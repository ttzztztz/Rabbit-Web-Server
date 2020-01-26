cmake_minimum_required(VERSION 3.10)
project(Web_Server)

set(CMAKE_CXX_STANDARD 17)

add_executable(Web_Server main.cpp request.cpp request.h handler.cpp handler.h helper.cpp helper.h response.cpp response.h epoll_helper.cpp epoll_helper.h)