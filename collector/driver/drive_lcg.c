#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include "driver_lcg.h"

#define DEVICE_NAME "driver_lcg"
#define LCG_MAJOR 666
#define LCG_DEV_NUM       MKDEV(LCG_MAJOR, 0)
#define LCG_DEV_CNT     1


static const struct file_operations g_lcg_fops = {
        .open = lcg_open,
        .release = lcg_release,
        .mmap = lcg_mmap,
        .owner = THIS_MODULE,
};

static int lcg_open(struct inode *inode, struct file *filp){
    size_t mmap_size = 0x10000;

    printk(KERN_INFO "LCG Device has been opened\n");

    filp->private_data = vmalloc(mmap_size);
    if (filp->private_data == NULL)
        return -1;

    return 0;
}

static int lcg_release(struct inode *inode, struct file *filp){
    return 0;
}

static int lcg_mmap(struct file *filp, struct vm_area_struct *vma){

    printk(KERN_INFO "LCG Device mmap\n");

    if (remap_pfn_range( vma, vma->vm_start, vmalloc_to_pfn(filp->private_data),
                         vma->vm_end - vma->vm_start, vma->vm_page_prot)
            )
    {
        printk(KERN_INFO "LCG Device mmap failed\n");
        return -EAGAIN;
    }
    printk(KERN_INFO "LCG Device mmap OK\n");

    return 0;
}

static int __init lcg_init(void)
{
    struct cdev *lcg_pcdev;
    struct class *lcg_pclass;
    unsigned int lcg_dev_major = 0;
    dev_t lcg_dev;

    int ret = alloc_chrdev_region(&lcg_dev, 0, 1, DEVICE_NAME);
    lcg_dev_major = MAJOR(lcg_dev);

    if (ret < 0 ){
        printk(KERN_INFO "alloc_chrdev_region error %d",ret);
    }

    printk(KERN_INFO "MAJOR %d\n", lcg_dev_major);

    lcg_pcdev = cdev_alloc();
    cdev_init(lcg_pcdev, &g_lcg_fops);

    ret = cdev_add(lcg_pcdev, LCG_DEV_NUM, LCG_DEV_CNT);
    if (ret) {
        printk(KERN_INFO "cdev_add error\n");
        return -1;
    }

    lcg_pclass = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(lcg_pclass)) {
        printk(KERN_ERR "can't register device %s class\n",DEVICE_NAME);
        return -1;
    }

    device_create(lcg_pclass, NULL, LCG_DEV_NUM, NULL, DEVICE_NAME);

    return 0;

}
static void __exit lcg_exit(void)
{
    return;
}

module_init(lcg_init)
module_exit(lcg_exit)
MODULE_LICENSE("GPL");
