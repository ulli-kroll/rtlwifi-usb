PWD = $(shell pwd)

export CONFIG_RTLWIFI_USB = m
export CONFIG_RTL8822BU = m
PLATFORM = PC

MAKE = make

ifeq ($(PLATFORM),PC)
KSRC = /lib/modules/$(shell uname -r)/build
SUBARCH := $(shell uname -m | sed -e s/i.86/i386/)
ARCH ?= $(SUBARCH)
endif

PHONY += all clean

all: modules

modules:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KSRC) M=$(PWD)  modules

strip:
	$(CROSS_COMPILE)strip $(MODULE_NAME).ko --strip-unneeded

installfw:
	mkdir -p /lib/firmware/rtlwifi
	cp -n firmware/* /lib/firmware/rtlwifi/.

.PHONY: modules clean

clean:
	rm -fr */*.mod.c */*.mod */*.o */.*.cmd
	rm -f *.mod.c *.o .*.cmd *.ko
	rm -f Module.symvers Module.markers modules.order
	rm -fr .tmp_versions

help:
	@echo "options :"
	@echo "modules		build this module"
	@echo "installfw	install firmware"
	@echo "clean		clean"
