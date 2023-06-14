obj-m:=call_os3.o
CURRENT_PATH :=$(shell pwd)
VERSION_NUM :=$(shell uname -r)
LINUX_PATH :=/usr/src/linux-headers-$(VERSION_NUM)
all :
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) modules
	rm *.order *.mod.c *.o 
clean :
	make -C $(LINUX_PATH) M=$(CURRENT_PATH) clean
