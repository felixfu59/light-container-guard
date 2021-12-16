//
// Created by foyjog on 11/8/21.
//

#ifndef LIGHT_CONTAINER_GUARD_KPROBE_LCG_H
#define LIGHT_CONTAINER_GUARD_KPROBE_LCG_H

#endif //LIGHT_CONTAINER_GUARD_KPROBE_LCG_H
int register_kprobe_open(void);
int unregister_kprobe_open(void);

int register_kprobe_bind(void);
int unregister_kprobe_bind(void);

int open_handler_pre(struct kprobe *p, struct pt_regs *regs);
int bind_handler_pre(struct kprobe *p, struct pt_regs *regs);