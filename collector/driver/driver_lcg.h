//
// Created by foyjog on 11/8/21.
//

#ifndef DRIVER_DRIVER_LCG_H
#define DRIVER_DRIVER_LCG_H

#endif //DRIVER_DRIVER_LCG_H
static int lcg_open(struct inode *inode, struct file *filp);
static int lcg_release(struct inode *inode, struct file *filp);
static int lcg_mmap(struct file *filp, struct vm_area_struct *vma);
static int register_all_kprobes(void);
static int unregister_all_kprobes(void);