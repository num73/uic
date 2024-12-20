<!--
 * @Author: gxl
 * @Date: 2024-12-19 19:33:23
 * @LastEditors: Xiaolong Guo
 * @LastEditTime: 2024-12-19 22:51:10
 * @FilePath: /uic/uic_design.md
 * @Description: UIC Design Doc.
-->

# Universal Inter Connect Library

## Goal

A universal inter connect library.

Providing homogeneous interfaces for heterogeneous Inter-Connect. 

## Structure



### Inter Connect Way (IC-Way)

IC-Way is the main concept of the library using in unix.

Every IC-Way has an unique id named `ic_way_id`, an ic-way means a connection channel. Several IC-Ways can be grouped. 
An IC-Way group usually have a same task, but they may have different bandwidth or latency. Ideally, we can always find the
best IC-Way to send message. The best means that the highest bandwidth or the lowest latency. If there is an accident happened
toward an IC-Way, other IC-Ways in the same group can help with the task.



### ID

* ui_way ID
* Server ID
* TCP ID (ip + port)

The id is an int, means that the unique identifier of a server connection.

### Priority

The priority is an int, the biggest first.

* Manual Priority Configuration.
* Automatic Configuration Priority.

### API

* register_ic_way
* unregister_ic_way
* uic_connect
* uic_disconnect
* uic_send
* uic_get

### Code Details

```c

struct ic_way{

    struct ic_way* next_ic_way;

    int64_t ic_way_id;
    int64_t group_id;
    int priority;
    // UIC_CONNECTED, UIC_LONELY
    int state;

    int (*connect)(void* attr);
    int (*disconnect)(void* attr);
    int (*get_data)(void* attr, char* buf, int size);
    int (*send_data)(void* attr, char* buf, int size);

    void* attr;
};


int register_ic_way(ic_way* _ic_way);
int unregister_ic_way(ic_way* _ic_way);


// return ic_way id or -1
int uic_connect(int server_id);

int uic_disconnect(int server_id);

int uic_send(int server_id, char* buf, int size);

// need ite
int uic_get(int server_id, char* buf, int size);

int uic_get_by_wid(int wid, char* buf, int size);

```


## Potential

parallel






