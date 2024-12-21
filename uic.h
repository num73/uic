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
    int64_t portal_id;
    int priority;

    // UIC_CONNECTED, UIC_LONELY
    int state;

    int (*connect)(void* attr);
    int (*disconnect)(void* attr);
    int (*get_data)(void* attr, char* buf, int len);
    int (*send_data)(void* attr, char* buf, int len);

    void* attr;
};

struct portal {

    int64_t portal_id;

    struct ic_way *ic_way_head;

    struct portal *next_portal;

    void *attr;
};


int register_ic_way(struct ic_way* _ic_way);

int unregister_ic_way(struct ic_way* _ic_way);


int uic_connect(int64_t _portal_id);

int uic_disconnect(int64_t _portal_id);

int uic_send_data(int64_t _portal_id, char* _buf, size_t _len);

// need ite
int uic_get_data(int64_t _portal_id, char* _buf, size_t _len);

int uic_get_data_by_wid(int64_t _wid, char* _buf, size_t _len);

#endif