<!--
 * @Author: gxl
 * @Date: 2024-12-19 19:33:23
 * @LastEditors: gxl
 * @LastEditTime: 2024-12-23 12:40:05
 * @FilePath: /uic/doc/design.md
 * @Description: UIC Design Doc.
-->

# Universal Inter Connect Library (UIC)

## UIC Goal

A universal inter connect library.

## UIC Motivation

 Two servers could be connected by Internet and CXL, they may also need to communicate to a third server which can not communicate with CXL. Usually, these two types of machines need different code to run a same application, and additional work is needed when the applicaion is migrated. UIC aims to provide homogeneous interfaces for heterogeneous Inter-Connect.

## UIC Features

1. Simple interfaces, only four interfaces which are needed to be implemented.
2. Flexible, UIC can be adapted to different communication protocols.
3. Optimized, UIC can choose the better communication way when two ways has a similar mission.
4. Hardware friendly, we implemented the UIC in C language to accommodate different hardware directly.

## UIC Design

### Conceptions

* **Room:** A room usually is the abstraction of a server or an application. 
* **Portal:** A room has several portals. A portal can connect to another portal or anything else which can handle data or send data.
* **Inter Connect Way(IC-Way)**: A portal has several ic-ways, and the ic-ways in the same portal have simliar characteristics.

### Room

A room means a closed space. Everything in the room can be seen and used directly without communicating with others. The thread owns the room is the data manipulator of UIC. 

In our design, a room usually stands for a server or an application. Room is the concept which makes the idea of UIC more clear. There is no specific room struct in our current implication of UIC. A room is a place which portals can be seen can used.

### Portal

A room can not see other rooms unless we add windows on the room. Portal could be the window and message deliver.

A room needs to communicate to others which are out the room needs one or more portals. Portal provides APIs for a room to send and receive data. When we need to send data to other servers, we just need to throw the data into the portal. If we want to know if there is data sended to the room, we just hire a listener to watch the portal. The programmer don't need to know how the portal send the data. Ideally, when we send data through a portal, the portal can alway choose the suitable way to send the data, and we can receive the data sent to us by each way connected to the portal.


```

### Inter Connect Way (IC-Way)

A portal has several Inter Connect Ways(IC-Ways), an ic-way means a connection channel. 
IC-Ways in the same portal usually have the same mission, they usually have similar characteristics. They can be replaced by each other.
Ideally, we can always find the best IC-Way in a portal to send a message. The best means that the highest bandwidth or the lowest latency. The IC-Ways are the implemention of the data sender or receiver. They are transparent to programmers.


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


## API

UIC only privide the necessary APIs used for transfering data.


* **register_portal(_portal)**: Add a portal to the room. 

* **register_ic_way(_ic_way, _portal_id)**: Add a IC-Way to the portal.

* **uic_send_data(_portal_id, _buf, _len)**: Send data through protal. 

* **uic_get_data(_portal_id, _buf, _len)**: Receive data from portal.

### Implemention

The UIC is implemented using C language considering the convenient calling of cxl drivers in the linux kernel. We have borrowed from the VFS method in the kernel and utilized function pointers to achieve polymorphism-like characteristics similar to those in object-oriented languages.


Room is the concept which makes the idea of UIC more clear. There is no specific room struct in our current implication of UIC.

We use a struct named portal to represent for a portal. Each portal has a unique portal_id to identify a portal. Also, there is a function pointer 
which can customize the strategy of selecting IC-Way. This function pointer is not necessary. If no need to customize the strategy, we have a default selection strategy which selecting IC-Way accorging to the IC-Way priority. The addidtional `void* portal_attr` is a free space to set value.


The IC-Way is also a struct. Each IC-Way has a ic_way_id, the pair (portal_id, ic_way_id) can identify a unique IC-Way. Each IC-Way has a priority which can be set when the IC-Way is registered. There are four function pointers needed to be implicated. The jobs of the four functions are connect, disconnect, get_data and send_data. The addidtional `void* portal_attr` is a free space to set value. 

The function pointers and the addidtional attr together make UIC flexible.

When use the UIC, we need to implicate an IC-Way and register it to a portal, and then we can use the unified interfaces to handle data.




## Potential

Our UIC designed for the CXL communication between servers, but the model UIC used has many other potentials:

IC-Ways in a portal have same mission, if we can use all the IC-Ways in a portal to send data parallelly. The preformance of data send can be improved significantly.

IC-Ways in a portal are designed to have the ability to replace each other, which means that if there is an accident happened toward an IC-Way, other IC-Ways in the same portal can help with the task.

In our current implemention, we need to set the priority, so the portal would know which IC-Way is the best one. Automatic Configuration Priority could be added.

Anyway, to improve the UIC, we need more real applications and demands.

