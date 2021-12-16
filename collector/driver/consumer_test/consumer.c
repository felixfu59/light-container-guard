//
// Created by foyjog on 11/8/21.
//
#include <linux/slab.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

static const int RING_BUFFER_LCG_SIZE = 8 * 1024 * 1024;

struct ring_buffer_lcg{
    char* buffer;
    unsigned int buffer_size;
    unsigned int write;
    unsigned int read;
    unsigned int data_size;
};


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

int main(){
    int fd = open("/dev/lcg",O_RDONLY);
    if(fd < 0){
        printf("fd < 0, error");
    }
    struct ring_buffer_lcg* rbl = (struct ring_buffer_lcg*) mmap(NULL, 0x1000 , PROT_READ , MAP_SHARED, fd, 0);
    if (rbl == MAP_FAILED)
    {
        perror("Failed to mmap: ");
        close(fd);
        return -1;
    }


    char* rbl_buffer = (char *) mmap(NULL, RING_BUFFER_LCG_SIZE, PROT_READ , MAP_SHARED, fd, 0);
    if (rbl_buffer == MAP_FAILED)
    {
        perror("Failed to mmap: ");
        close(fd);
        return -1;
    }
    rbl->buffer = rbl_buffer;

    char buffer[1000];

    ring_buffer_read(rbl, buffer, 1000);

    for (int ii=0;ii<100;ii++){
        printf("%c",buffer[ii]);
    }

    close(fd);
    return 0;
}