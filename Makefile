
obj-m:= example_devicedriver.o

KDIR := /lib/modules/5.10.0-22-amd64/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
install:
	$(MAKE) -C $(KDIR) M=$(PWD) module_install
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clear

