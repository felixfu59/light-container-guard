//
// Created by foyjog on 11/8/21.
//
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include "kprobe_lcg.h"
#include "event_lcg.h"
#include "utils_lcg.h"

extern struct ring_buffer_lcg* g_event_ring_buffer;

static char g_magic_number[4] = {0xde,0xad,0xbe,0xef};

struct kprobe g_kp_open = {
        .symbol_name = "__x64_sys_openat",
        .pre_handler = open_handler_pre,
};

static struct kprobe g_kp_bind = {
        .symbol_name    = "__x64_sys_bind",
        .pre_handler = bind_handler_pre,
};

int open_handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    struct event_open *e_open;
    struct events_lcg *e_event;
    char* __user u_file_name;
    char* file_name;
    int file_name_len;

    int copy_count;

    u_file_name = (char *)((struct pt_regs *)(regs->di))->si;
    file_name_len = strnlen_user(u_file_name, PATH_MAX);

    e_open =  (struct event_open*)vmalloc(sizeof(struct event_open) + (file_name_len + 1) * sizeof(char));
    e_open->flags = 666;
    e_open->mode = 666;

    file_name = vmalloc((file_name_len + 1) * sizeof(char));
    copy_count =  copy_from_user(file_name, (char __user *)u_file_name, file_name_len);

    memcpy(e_open->filename, file_name, file_name_len + 1);

    e_event = (struct events_lcg*) vmalloc(sizeof(struct events_lcg));
    memcpy(e_event->magic_number , &g_magic_number, 4);
    memcpy(e_event->common , current->comm, 16);
    e_event->uid = current->real_cred->uid.val;
    e_event->pid = current->pid;
    e_event->ppid = current->real_parent->tgid;
    e_event->pns = current->nsproxy->pid_ns_for_children->ns.inum;
    e_event->event = (char*) e_open;
    e_event->event_length = sizeof(struct events_lcg) + sizeof(struct event_open) + (file_name_len + 1) * sizeof(char);

    if(g_event_ring_buffer != NULL){
        ring_buffer_write(g_event_ring_buffer, (char*)e_event ,sizeof(struct events_lcg) - sizeof (char*));
        ring_buffer_write(g_event_ring_buffer, (char*)e_open,sizeof(struct event_open) + (file_name_len + 1) * sizeof(char));
    }

    return 0;
}

int bind_handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    printk(KERN_INFO "bind_handler_pre");
    return 0;
}

int register_kprobe_open(void){
    int ret = 0;

    ret = register_kprobe(&g_kp_open);
    if (ret < 0) {
        printk(KERN_INFO "register_open failed, returned %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "register_open kprobe at %p\n", g_kp_open.addr);
    return 0;
}

int register_kprobe_bind(void){
    int ret = 0;

    ret = register_kprobe(&g_kp_bind);
    if (ret < 0) {
        printk(KERN_INFO "register_bind failed, returned %d\n", ret);
        return ret;
    }

    printk(KERN_INFO "register_bind kprobe at %p\n", g_kp_bind.addr);
    return 0;
}

int unregister_kprobe_open(void){
    unregister_kprobe(&g_kp_open);
    printk(KERN_INFO "unregister_open kprobe at %p\n", g_kp_open.addr);
    return 0;
}

int unregister_kprobe_bind(void){
    unregister_kprobe(&g_kp_bind);
    printk(KERN_INFO "unregister_bind kprobe at %p\n", g_kp_bind.addr);
    return 0;
}



