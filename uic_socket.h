/*
* @Author: gxl
 * @Date: 2024-12-20 12:14:19
 * @LastEditors: gxl
 * @LastEditTime: 2024-12-20 13:16:35
 * @FilePath: /uic/uic_socket.h
 * @Description:
 */
#ifndef _UIC_SOCKET_H
#define _UIC_SOCKET_H

#include <arpa/inet.h>

#define UIC_SOCKET_SERVER 1
#define UIC_SOCKET_CLIENT 2

struct uic_socket_attr {
    int flag;
    int server_fd;
    int socket_fd;
    char* ip;
    int port;
    struct sockaddr_in address;
    int opt;
    int addr_len;

};

void uic_socket_init(int _flag, int _portal_id, char* _ip, int _port);

#endif

