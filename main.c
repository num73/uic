/*
* @Author: Xiaolong Guo
 * @Date: 2024-12-19 20:34:40
 * @LastEditors: Xiaolong Guo
 * @LastEditTime: 2024-12-26 10:13:57
 * @Description: Main
 */


#include "uic.h"
#include "uic_log.h"
#include "uic_socket.h"
#include <stdio.h>
#include <string.h>
char buf[1024];
int port = 5323;
int main(int argc, char *argv[]) {

    if(argc < 2) {
        uic_error("Need to specify the role of the program, 0 for server, 1 for client");
        return -1;
    }
    if(strcmp(argv[1], "0") == 0) {
        uic_socket_init(UIC_SOCKET_SERVER, UIC_LOCAL_PORTAL, "127.0.0.1", port);
        uic_connect(UIC_LOCAL_PORTAL);


        while(1) {
            uic_get_data(UIC_LOCAL_PORTAL, buf, 1024);
            if(strlen(buf) > 0) {
                uic_info("Received message: %s", buf);
            }
            if(buf[0] == '!') {
                break;
            }
        }
    } else {
        uic_socket_init(UIC_SOCKET_CLIENT, UIC_LOCAL_PORTAL, "127.0.0.1", port);
        uic_connect(UIC_LOCAL_PORTAL);

        while(1) {
            printf("Please input message(`!` start to exit): ");
            fgets(buf, 1024, stdin);
            uic_send_data(UIC_LOCAL_PORTAL, buf, strlen(buf));
            if(buf[0] == '!') {
                break;
            }
        }

    }
    

    uic_disconnect(UIC_LOCAL_PORTAL);

    return 0;
}