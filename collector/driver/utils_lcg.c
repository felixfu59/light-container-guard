//
// Created by foyjog on 11/8/21.
//

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#include "utils_lcg.h"

struct ring_buffer_lcg* ring_buffer_lcg_init(int ring_buffer_size){
    struct ring_buffer_lcg* rbl;
    rbl = (struct ring_buffer_lcg*) vmalloc(sizeof (struct ring_buffer_lcg));
    if(rbl == NULL){
        printk(KERN_INFO "rbl kzalloc failed!");
        return NULL;
    }
    rbl->write = 0;
    rbl->read = 0;
    rbl->buffer_size = ring_buffer_size;
    rbl->buffer = vmalloc(ring_buffer_size * sizeof (char));
    if(rbl->buffer == NULL){
        printk(KERN_INFO "ring buffer kzalloc failed!");
        return NULL;
    }
    return rbl;
}


void ring_buffer_write(struct ring_buffer_lcg* rbl, char* buffer_write, int write_size){

    rbl->data_size += write_size;
    if(rbl->data_size > rbl->buffer_size){
        rbl->data_size = rbl->buffer_size;
    }

    if ((rbl->write + write_size) <= rbl->buffer_size){
        memcpy(rbl->buffer + rbl->write, buffer_write,write_size);
        rbl->write += write_size;
    } else{
        memcpy(rbl->buffer + rbl->write, buffer_write, rbl->buffer_size - rbl->write);
        memcpy(rbl->buffer, buffer_write + rbl->buffer_size - rbl->write,write_size - (rbl->buffer_size - rbl->write));
        rbl->write += write_size;
        rbl->write %= rbl->buffer_size;
    }
}

void ring_buffer_read(struct ring_buffer_lcg* rbl, char* buffer_read, int read_size){

    if(rbl->data_size >= read_size) {
        rbl->data_size -= read_size;
    }else{
        read_size = rbl->data_size;
        rbl->data_size = 0;
        if(read_size == 0){
            return;
        }
    }

    if ((rbl->read + read_size) <= rbl->buffer_size){
        memcpy(buffer_read, rbl->buffer+rbl->read, read_size);
        rbl->read += read_size;
    }else{
        memcpy(buffer_read, rbl->buffer+rbl->read, rbl->buffer_size - rbl->read);
        memcpy(buffer_read + rbl->buffer_size - rbl->read, rbl->buffer, read_size - (rbl->buffer_size - rbl->read));
        rbl->read %= rbl->buffer_size;
    }
}