#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include "driver_lcg.h"
#include "kprobe_lcg.h"
#include "utils_lcg.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("foyjog#foyjog@163.com");

#define DEVICE_NAME "lcg"
#define LCG_MAJOR 666
#define LCG_DEV_NUM       MKDEV(LCG_MAJOR, 0)
#define LCG_DEV_CNT     1

static struct cdev *g_lcg_pcdev;
static struct class *g_lcg_pclass;
static unsigned int g_lcg_dev_major = 0;
static dev_t g_lcg_dev;

static const int RING_BUFFER_LCG_SIZE = 8 * 1024 * 1024;
struct ring_buffer_lcg* g_event_ring_buffer = NULL;

static const struct file_operations g_lcg_fops = {
        .open = lcg_open,
        .release = lcg_release,
        .mmap = lcg_mmap,
        .owner = THIS_MODULE,
};

static int lcg_open(struct inode *inode, struct file *filp){

    printk(KERN_INFO "LCG Device has been opened\n");

    //register kprobes in kernel
    register_all_kprobes();

    g_event_ring_buffer = ring_buffer_lcg_init(RING_BUFFER_LCG_SIZE);
    if (g_event_ring_buffer == NULL)
        return -1;

    filp->private_data = g_event_ring_buffer;

    return 0;
}

static int lcg_release(struct inode *inode, struct file *filp){
    return 0;
}

static int lcg_mmap(struct file *filp, struct vm_area_struct *vma){

    struct ring_buffer_lcg* ring_buffer;
    int vma_length = vma->vm_end - vma->vm_start;

    printk(KERN_INFO "LCG Device mmap\n");

    if (vma_length == 0x1000){
        if (remap_pfn_range(vma, vma->vm_start, vmalloc_to_pfn(filp->private_data), vma_length, vma->vm_page_prot)){
            printk(KERN_INFO "LCG Device mmap failed\n");
            return -EAGAIN;
        }
    }else if (vma_length == (RING_BUFFER_LCG_SIZE)){
        ring_buffer = (struct ring_buffer_lcg*)(filp->private_data);
        if (remap_pfn_range(vma, vma->vm_start, vmalloc_to_pfn(ring_buffer->buffer), vma_length, vma->vm_page_prot)){
            printk(KERN_INFO "LCG Device mmap failed\n");
            return -EAGAIN;
        }
    }else{
        printk(KERN_INFO "LCG Device mmap size error\n");
        return -1;
    }

    printk(KERN_INFO "LCG Device mmap OK\n");

    return 0;
}

static int register_all_kprobes(void){
    register_kprobe_open();
    register_kprobe_bind();

    return 0;
}

static int unregister_all_kprobes(void){
    unregister_kprobe_open();
    unregister_kprobe_bind();

    return 0;
}

static int __init lcg_init(void){
    int ret = alloc_chrdev_region(&g_lcg_dev, 0, 1, DEVICE_NAME);
    g_lcg_dev_major = MAJOR(g_lcg_dev);

    if (ret < 0 ){
        printk(KERN_INFO "alloc_chrdev_region error %d",ret);
    }

    printk(KERN_INFO "MAJOR %d\n", g_lcg_dev_major);

    g_lcg_pcdev = cdev_alloc();
    cdev_init(g_lcg_pcdev, &g_lcg_fops);

    ret = cdev_add(g_lcg_pcdev, LCG_DEV_NUM, LCG_DEV_CNT);
    if (ret) {
        printk(KERN_INFO "cdev_add error\n");
        return -1;
    }

    g_lcg_pclass = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(g_lcg_pclass)) {
        printk(KERN_ERR "can't register device %s class\n",DEVICE_NAME);
        return -1;
    }

    device_create(g_lcg_pclass, NULL, LCG_DEV_NUM, NULL, DEVICE_NAME);

    return 0;

}
static void __exit lcg_exit(void)
{
    device_destroy(g_lcg_pclass, LCG_DEV_NUM);
    class_destroy(g_lcg_pclass);
    cdev_del(g_lcg_pcdev);
    unregister_chrdev_region(LCG_DEV_NUM, 1);

    unregister_all_kprobes();

    vfree(g_event_ring_buffer->buffer);
    vfree(g_event_ring_buffer);
    g_event_ring_buffer = NULL;

    return;
}

module_init(lcg_init)
module_exit(lcg_exit)
MODULE_LICENSE("GPL");
