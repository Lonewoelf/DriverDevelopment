KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
MAKE = /usr/bin/make

%.ko : %.c
	$(MAKE) obj-m=$(*).o -C $(KDIR) M=$(PWD) modules

clean:
	rm -rfv *ko *cmd* .* *order *symvers *.o* *.mod*

install:
	sudo insmod ARW_driver.ko;
	sudo mknod /dev/ARW_driver c 243 0 -m 0666;

uninstall:
	sudo rmmod ARW_driver;
	sudo rm /dev/ARW_driver;