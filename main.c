/*
* @Author: Xiaolong Guo
 * @Date: 2024-12-19 20:34:40
 * @LastEditors: gxl
 * @LastEditTime: 2024-12-20 12:40:17
 * @Description: Main
 */


#include "uic.h"
#include "uic_socket.h"
#include <stdio.h>
#include <string.h>
char buf[1024];

int main() {

    uic_socket_init(UIC_SOCKET_CLIENT, UIC_LOCAL_PORTAL, "127.0.0.1", 8888);

    uic_connect(UIC_LOCAL_PORTAL);


    //uic_get_data(UIC_LOCAL_GROUP, buf, sizeof(buf));
    //printf("%s\n", buf);

    fgets(buf, 1024, stdin);
    uic_send_data(UIC_LOCAL_PORTAL, buf, strlen(buf)*sizeof(char));

    uic_disconnect(0);

    return 0;
}