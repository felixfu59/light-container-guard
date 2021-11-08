obj-m := driver_lcg.o

KDIR = /lib/modules/$(shell uname -r)/build

CUR_DIR := $(shell pwd)

all:
	make -C $(KDIR) M=$(CUR_DIR) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.mod .*.cmd *.symvers modul*

