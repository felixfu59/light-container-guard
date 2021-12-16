lcg-y += driver_lcg.o kprobe_lcg.o event_lcg.o utils_lcg.o
obj-m := lcg.o

KDIR = /lib/modules/$(shell uname -r)/build
KQEMUDIR = /home/foyjog/Desktop/kernel/linux-5.4.89-qemu
CUR_DIR := $(shell pwd)

all:
	make -C $(KDIR) M=$(CUR_DIR) modules
qemu:
	make -C $(KQEMUDIR) M=$(CUR_DIR) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.mod .*.cmd *.symvers modul*

