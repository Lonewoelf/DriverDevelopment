KDIR = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)
MAKE = /usr/bin/make

%.ko : %.c
	$(MAKE) obj-m=$(*).o -C $(KDIR) M=$(PWD) modules

clean:
	rm -rfv *ko *cmd* .* *order *symvers *.o* *.mod*

install:
	sudo insmod RW_driver.ko;
	sudo mknod /dev/RW_driver c 241 0 -m 0666;

unistall:
	sudo rmmod RW_driver;
	sudo rm /dev/RW_driver;