//
// Created by foyjog on 11/8/21.
//

#ifndef LIGHT_CONTAINER_GUARD_UTILS_LCG_H
#define LIGHT_CONTAINER_GUARD_UTILS_LCG_H

#endif //LIGHT_CONTAINER_GUARD_UTILS_LCG_H

struct ring_buffer_lcg* ring_buffer_lcg_init(int ring_buffer_size);
void ring_buffer_write(struct ring_buffer_lcg* rbl, char* buffer_write, int write_size);
void ring_buffer_read(struct ring_buffer_lcg* rbl, char* buffer_read, int read_size);

struct ring_buffer_lcg{
    char* buffer;
    unsigned int buffer_size;
    unsigned int write;
    unsigned int read;
    unsigned int data_size;
};

