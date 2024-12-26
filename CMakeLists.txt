cmake_minimum_required(VERSION 2.30)
project(uic C)

set(CMAKE_C_STANDARD 99)

add_executable(uic main.c
        uic.c
        uic.h
        uic_socket.h
        uic_socket.c
        uic_log.h)
