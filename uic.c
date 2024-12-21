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
#include <stdlib.h>

#include "uic_log.h"


struct portal* portals_head;

static struct portal* portal_by_portal_id(const int64_t _portal_id) {
    struct portal* p = portals_head;
    while (p) {
        if (p->portal_id == _portal_id) {
            return p;
        }
        p = p->next_portal;
    }
    return NULL;
}

struct ic_way* generic_selected_ic_way(const struct portal* _portal) {

    struct ic_way* ret = NULL;
    struct ic_way* p = _portal->ic_way_head;
    int64_t pri = UIC_PRI_MIN;
    while (p) {
        if (p->priority > pri) {
            pri = p->priority;
            ret = p;
        }
        p = p->next_ic_way;
    }

    return ret;
}

static struct ic_way* select_ic_way_in_portal(const struct portal* _portal) {
    uic_dbg("selecting ic_way in portal: portal_id: %ld", _portal->portal_id);
    if (_portal->selected_ic_way == NULL) {
        return generic_selected_ic_way(_portal);
    }
    return _portal->selected_ic_way(_portal);
}

static struct ic_way* select_ic_way(int64_t _portal_id) {

    struct portal* portal = portal_by_portal_id(_portal_id);
    if (portal == NULL) {
        return NULL;
    }
    return select_ic_way_in_portal(portal);

}


int register_portal(struct portal* _portal) {

    struct portal* p = portals_head;
    if (p == NULL) {
        portals_head = _portal;
        return UIC_SUCCESS;
    }
    while (p->next_portal) {
        p = p->next_portal;
    }
    p->next_portal= _portal;

    return UIC_SUCCESS;
}

int register_ic_way(struct ic_way *_ic_way, int64_t _portal_id) {

    struct portal* portal = portal_by_portal_id(_portal_id);
    if (portal == NULL) {
        uic_dbg("portal_id not find, new portal will be added, portal_id: %ld", _portal_id);
        portal = malloc(sizeof(struct portal));
        portal->ic_way_head =  _ic_way;
        portal->portal_id = _portal_id;
        register_portal(portal);
        return UIC_SUCCESS;
    }

    struct ic_way *p = portal->ic_way_head;

    if (p == NULL) {
        portal->ic_way_head = _ic_way;
        return UIC_SUCCESS;
    }
    while (p->next_ic_way) {
        p = p->next_ic_way;
    }
    p->next_ic_way = _ic_way;
}


int unregister_ic_way(int64_t _ic_way_id, int64_t _portal_id){

    struct portal* portal = portal_by_portal_id(_portal_id);
    if (portal == NULL) {
        return UIC_FAIL;
    }

    struct ic_way* p = portal->ic_way_head;
    struct ic_way* q = NULL;

    if (p == NULL) {
        return UIC_FAIL;
    }
    if (p->ic_way_id == _ic_way_id) {
        portal->ic_way_head = p->next_ic_way;
        return UIC_SUCCESS;
    }
    while (p->next_ic_way) {
        if (p->next_ic_way->ic_way_id == _ic_way_id) {
            q = p->next_ic_way;
            p->next_ic_way = q->next_ic_way;
            return UIC_SUCCESS;
        }
        p = p->next_ic_way;
    }
    return UIC_FAIL;
}



int uic_connect(int64_t _portal_id) {
    uic_dbg("Connecting to portal %d ...\n", _portal_id);
    struct ic_way* p = select_ic_way(_portal_id);

    if (p == NULL) {
        return UIC_FAIL;
    }
    if(p->connect == NULL) {
        return UIC_FAIL;
    }
    if(p->connect(p->ic_way_attr) == UIC_SUCCESS) {
        p->state = UIC_CONNECTED;
        return UIC_SUCCESS;
    } else {
        return UIC_FAIL;
    }

}

int uic_disconnect(int64_t _portal_id) {

    struct portal* portal = portal_by_portal_id(_portal_id);

    struct ic_way* p = portal->selected_ic_way(portal);

    if (p == NULL) {
        return UIC_FAIL;
    }
    if(p->disconnect == NULL) {
        return UIC_FAIL;
    }
    if(p->disconnect(p->ic_way_attr) == UIC_SUCCESS) {
        p->state = UIC_LONELY;
        return UIC_SUCCESS;
    } else {
        return UIC_FAIL;
    }
}

int uic_send_data(int64_t _portal_id, char* _buf, size_t _len) {
    uic_dbg("Sending data to portal %ld ...\n", _portal_id);
    struct ic_way* p = select_ic_way(_portal_id);

    if (p == NULL) {
        uic_dbg("Send null ic_way\n");
        return UIC_FAIL;
    }
    if(p->send_data == NULL) {
        return UIC_FAIL;
    }
    return p->send_data(p->ic_way_attr, _buf, _len);
}

// All ic-way in portal
int uic_get_data(int64_t _portal_id, char* _buf, size_t _len) {

    struct portal* portal = portal_by_portal_id(_portal_id);
    struct ic_way* p = portal->ic_way_head;
    while (p) {
        if(p->get_data != NULL && p->state == UIC_CONNECTED && p->get_data(p->ic_way_attr, _buf, _len) == UIC_SUCCESS) {
                    return UIC_SUCCESS;
        }
        p = p->next_ic_way;
    }
    if (p == NULL) {
        return UIC_FAIL;
    }
    return p->get_data(p->ic_way_attr, _buf, _len);
}

int uic_disconnect_all(int64_t _portal_id) {
    struct portal* portal = portal_by_portal_id(_portal_id);

    struct ic_way* p = portal->ic_way_head;
    while (p) {
        if (p->state == UIC_CONNECTED && p->disconnect != NULL) {
            p->disconnect(p->ic_way_attr);
        }
    }
    return UIC_SUCCESS;
}