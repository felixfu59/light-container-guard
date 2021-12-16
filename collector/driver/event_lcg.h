//
// Created by foyjog on 11/8/21.
//

#ifndef LIGHT_CONTAINER_GUARD_EVENT_LCG_H
#define LIGHT_CONTAINER_GUARD_EVENT_LCG_H

#endif //LIGHT_CONTAINER_GUARD_EVENT_LCG_H

struct events_lcg{
    char magic_number[4];
    int event_length;
    int uid;
    int pid;
    int ppid;
    char common[16];
    int pns;

    char *event;
};

struct event_open{
    int flags;
    int mode;

    char filename[0];
};