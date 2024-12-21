<!--
 * @Author: gxl
 * @Date: 2024-12-19 19:33:23
 * @LastEditors: Xiaolong Guo
 * @LastEditTime: 2024-12-20
 * @FilePath: /uic/uic_design.md
 * @Description: UIC Design Doc.
-->

# Universal Inter Connect Library

@author num73

## Goal

A universal inter connect library.

Providing homogeneous interfaces for heterogeneous Inter-Connect. 

## Structure

### Room

A room usually stands for a server or a client.

### Portal

A room has several portals. A portal can connect to another portal or anything else which can handle data.

Portal has a unique portal_id. The function `select_ic_way` can be set. If there is no `select_ic_way` in portal means that
`select_ic_way` is NULL, `generic_selected_ic_way` will be used.


```c

struct portal {

    int64_t portal_id;

    char* portal_name;

    struct ic_way *ic_way_head;

    struct portal *next_portal;

    struct ic_way* (*selected_ic_way)(const struct portal* portal);

    void *portal_attr;
};

```

### Inter Connect Way (IC-Way)

A portal has several ic-ways, an ic-way means a connection channel. Ideally, we can always find the
best IC-Way in a portal to send a message. The best means that the highest bandwidth or the lowest latency. 
If there is an accident happened toward an IC-Way, other IC-Ways in the same portal can help with the task.

IC-Way is the main concept of the library using in unix.

Every IC-Way has an unique id pair (`portal_id`, `ic_way_id`), an ic-way means a connection channel. Several IC-Ways can be grouped. 
An IC-Way group usually have a same task, but they may have different bandwidth or latency. Ideally, we can always find the
best IC-Way to send message. The best means that the highest bandwidth or the lowest latency. If there is an accident happened
toward an IC-Way, other IC-Ways in the same group can help with the task.

```c
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
```

### ID

* portal_id
* ic_way ID

The id is an int64, means that the unique identifier.

### Priority

The priority is an int64, the biggest first.

* Manual Priority Configuration.
* Automatic Configuration Priority. // todo

### API


* register_portal(_portal);

* register_ic_way(_ic_way, _portal_id);

* unregister_ic_way(_ic_way_id, _portal_id);

* uic_connect(_portal_id);

* uic_connect_all(_portal_id); // todo

* uic_disconnect(_portal_id);

* uic_disconnect_all(_portal_id); 

* uic_send_data(_portal_id, _buf, _len);

* uic_get_data(_portal_id, _buf, _len);

* uic_get_data_through_way(_portal_id, _wid, _buf, _len); // todo

## How to Use

The `socket.c` and `socket.h` are the example of uic.

Implement an IC-Way and register it!

## Potential

parallel






