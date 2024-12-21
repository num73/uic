/*
 * @Author: gxl
 * @Date: 2024-12-20 12:25:28
 * @LastEditors: gxl
 * @LastEditTime: 2024-12-20 13:22:25
 * @FilePath: /uic/uic_socket.c
 * @Description:
 */

#include "uic_socket.h"
#include "uic.h"
#include "uic_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int64_t wid_tag = 0;


int uic_socket_connect(void* attr) {
    struct uic_socket_attr *uca = (struct uic_socket_attr*)attr;

    if (uca->flag == UIC_SOCKET_SERVER) {
        uic_dbg("uic_socket_connect: server detected\n");
        if ((uca->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(uca->server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &uca->opt, sizeof(uca->opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        if (bind(uca->server_fd, (struct sockaddr *)&uca->address, sizeof(uca->address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(uca->server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        uic_info("Server is listening port: %d...\n", uca->port);

        if ((uca->socket_fd = accept(uca->server_fd, (struct sockaddr *)&uca->address, (socklen_t*)&uca->addr_len)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        uic_info("Connect to client successfully\n");

    } else if (uca->flag == UIC_SOCKET_CLIENT) {
        uic_dbg("uic_socket_connect: client detected\n");
        uic_dbg("ip: %s\n", uca->ip);
        if ((uca->socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            uic_error("\n Socket creation error \n");
            return -1;
        }
        if (inet_pton(AF_INET, uca->ip, &uca->address.sin_addr) <= 0) {
            uic_error("\nInvalid address/ Address not supported \n");
            return -1;
        }
        if (connect(uca->socket_fd, (struct sockaddr *)&uca->address, sizeof(uca->address)) < 0) {
            perror("connect failed");
            exit(EXIT_FAILURE);
        }
    }

    return UIC_SUCCESS;

}

int uic_socket_disconnect(void* attr) {
    struct uic_socket_attr* p = (struct uic_socket_attr*)attr;
    close(p->socket_fd);
    close(p->server_fd);
    return UIC_SUCCESS;
}


int uic_socket_send_data(void* attr, char* buf, size_t _len) {
    uic_dbg("sending data...: data: %s, len: %d\n", buf, _len);
    struct uic_socket_attr* p = (struct uic_socket_attr*)attr;

    send(p->socket_fd, buf, _len, 0);
    return UIC_SUCCESS;
}

int uic_socket_get_data(void* attr, char* buf, size_t _len) {

    uic_dbg("uic_socket_get_data\n");
    struct uic_socket_attr* p = (struct uic_socket_attr*)attr;
    read(p->socket_fd, buf, _len);

    return UIC_SUCCESS;
}

void uic_socket_init(int _flag, int _portal_id, char* _ip, int _port) {

    uic_info("uic_socket_init\n");
    uic_info("uic_socket_init, ip: %s\n", _ip);
    struct uic_socket_attr* uca = (struct uic_socket_attr*)malloc(sizeof(struct uic_socket_attr));
    struct ic_way* icw = (struct ic_way* )malloc(sizeof(struct ic_way));
    uca->flag = _flag;
    uca->opt = 1;
    uca->port = _port;
    uca->addr_len = sizeof(uca->address);

    uca->address.sin_family = AF_INET;

    //uca.address.sin_addr.s_addr = INADDR_ANY;

    uca->ip = malloc(sizeof(char)*strlen(_ip));
    memcpy(uca->ip, _ip, strlen(_ip));
    uca->address.sin_port = htons(_port);

    icw->ic_way_id = wid_tag++;
    icw->priority = 0;
    icw->state = UIC_LONELY;
    icw->connect = uic_socket_connect;
    icw->disconnect = uic_socket_disconnect;
    icw->get_data = uic_socket_get_data;
    icw->send_data = uic_socket_send_data;
    icw->ic_way_attr = uca;
    register_ic_way(icw, _portal_id);
    uic_info("uic_socket register successful!\n");
}