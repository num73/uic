/*
 * @Author: gxl
 * @Date: 2024-12-19 20:34:40
 * @LastEditors: gxl
 * @LastEditTime: 2024-12-20 13:07:04
 * @Description: Universal Inter Connect Library
 */

#ifndef _UIC_H
#define _UIC_H

#include <stdint.h>
#include <unistd.h>

// uic version.
#define UIC_VERSION "0.0.1"
//------

#define UIC_LONELY 0
#define UIC_CONNECTED 1

#define UIC_SUCCESS 1
#define UIC_FAIL (-1)

#define UIC_ANY_PORTAL 0
#define UIC_LOCAL_PORTAL 1

#define UIC_PRI_MIN (-0x3f3f3f3f3f3f3f3f)


struct ic_way{

    struct ic_way* next_ic_way;

    int64_t ic_way_id;
    //int64_t portal_id;
    int64_t priority;

    // UIC_CONNECTED, UIC_LONELY
    int state;

    int (*connect)(void* ic_way_attr);
    int (*disconnect)(void* ic_way_attr);
    int (*get_data)(void* ic_way_attr, char* buf, size_t len);
    int (*send_data)(void* ic_way_attr, char* buf, size_t len);

    void* ic_way_attr;
};

struct portal {

    int64_t portal_id;

    char* portal_name;

    struct ic_way *ic_way_head;

    struct portal *next_portal;

    struct ic_way* (*selected_ic_way)(const struct portal* portal);

    void *portal_attr;
};


int register_portal(struct portal* _portal);

int register_ic_way(struct ic_way *_ic_way, int64_t _portal_id);

int unregister_ic_way(int64_t _ic_way_id, int64_t _portal_id);

int uic_connect(int64_t _portal_id);

int uic_connect_all(int64_t _portal_id); // todo

int uic_disconnect(int64_t _portal_id);

int uic_disconnect_all(int64_t _portal_id);

int uic_send_data(int64_t _portal_id, char* _buf, size_t _len);

// need ite
int uic_get_data(int64_t _portal_id, char* _buf, size_t _len);

int uic_get_data_through_way(int64_t _portal_id, int64_t _wid, char* _buf, size_t _len); // todo

#endif