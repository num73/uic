/*
 * @Author: gxl
 * @Date: 2024-12-20 09:08:53
 * @LastEditors: gxl
 * @LastEditTime: 2024-12-20 12:01:43
 * @Description: Universal Inter Connect Library
 */
#include "uic.h"
#include <stdint.h>
#include <stdio.h>
#include "uic_log.h"

struct ic_way* ic_ways_head;

static struct ic_way* choice_ic_way(int64_t _group_id) {

    int64_t priority = UIC_PRI_MIN;
    struct ic_way* ret = NULL;
    struct ic_way* p = ic_ways_head;

    while (p) {
        if (p->group_id == _group_id) {
            if(p->priority > priority) {
                priority = p->priority;
                ret = p;
            }
        }
        uic_dbg("Find ic way successfully, ic way id %ld:\n", p->ic_way_id);
        p = p->next_ic_way;
    }
    return ret;

}

static struct ic_way* ic_way_by_way_id(int64_t _ic_way_id) {
    struct ic_way* p = ic_ways_head;
    while (p) {
        if (p->ic_way_id == _ic_way_id) {
            return p;
        }
        p = p->next_ic_way;
    }
    return NULL;
}


// return an ic_way in the group of _group_id
static struct ic_way* ic_way_by_groupid(int64_t _group_id) {
    struct ic_way* p = ic_ways_head;
    while (p) {
        if (p->group_id == _group_id) {
            return p;
        }
        p = p->next_ic_way;
    }
    return NULL;
}

int register_ic_way(struct ic_way *_ic_way) {

    struct ic_way* p = ic_ways_head;

    if (p == NULL) {
        ic_ways_head = _ic_way;
        return UIC_SUCCESS;
    }
    while (p->next_ic_way) {
        p = p->next_ic_way;
    }
    p->next_ic_way = _ic_way;
    return UIC_SUCCESS;
}


int unregister_ic_way(struct ic_way* _ic_way) {
    struct ic_way* p = ic_ways_head;
    struct ic_way* q = NULL;

    if (p == NULL) {
        return UIC_FAIL;
    }
    if (p == _ic_way) {
        ic_ways_head = p->next_ic_way;
        return UIC_SUCCESS;
    }
    while (p->next_ic_way) {
        if (p->next_ic_way == _ic_way) {
            q = p->next_ic_way;
            p->next_ic_way = q->next_ic_way;
            return UIC_SUCCESS;
        }
        p = p->next_ic_way;
    }
    return UIC_FAIL;
}




int uic_connect(int64_t _server_id) {
    uic_dbg("Connecting to server %d ...\n", _server_id);
    struct ic_way* p = choice_ic_way(_server_id);

    uic_dbg("Get server successfully!\n", _server_id);
    if (p == NULL) {
        return UIC_FAIL;
    }
    if(p->connect == NULL) {
        return UIC_FAIL;
    }
    if(p->connect(p->attr) == UIC_SUCCESS) {
        p->state = UIC_CONNECTED;
        return UIC_SUCCESS;
    } else {
        return UIC_FAIL;
    }

}

int uic_disconnect(int64_t _server_id) {


    struct ic_way* p = choice_ic_way(_server_id);

    if (p == NULL) {
        return UIC_FAIL;
    }
    if(p->disconnect == NULL) {
        return UIC_FAIL;
    }
    if(p->disconnect(p->attr) == UIC_SUCCESS) {
        p->state = UIC_LONELY;
        return UIC_SUCCESS;
    } else {
        return UIC_FAIL;
    }
}

int uic_send(int64_t _server_id, char* _buf, size_t _len) {
    uic_dbg("Sending data to server %d ...\n", _server_id);
    struct ic_way* p = choice_ic_way(_server_id);

    if (p == NULL) {
        uic_dbg("Send null ic_way\n");
        return UIC_FAIL;
    }
    if(p->send_data == NULL) {
        return UIC_FAIL;
    }
    return p->send_data(p->attr, _buf, _len);
}


int uic_get_data(int64_t _group_id, char* _buf, size_t _len) {
    struct ic_way* p = ic_ways_head;
    while (p) {
        if (p->group_id == _group_id) {
            if(p->get_data != NULL && p->state == UIC_CONNECTED && p->get_data(p->attr, _buf, _len) == UIC_SUCCESS) {
                    return UIC_SUCCESS;
            }
        }
        p = p->next_ic_way;

    }

    return p->get_data(p->attr, _buf, _len);
}

int uic_get_data_by_wid(int64_t _wid, char* _buf, size_t _len) {
    struct ic_way* p = ic_way_by_way_id(_wid);

    if (p == NULL) {
        return UIC_FAIL;
    }
    if(p->get_data == NULL) {
        return UIC_FAIL;
    }
    return p->get_data(p->attr, _buf, _len);
}