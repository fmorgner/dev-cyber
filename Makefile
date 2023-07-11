ifneq ($(KERNELRELEASE),)
include Kbuild
else
KDIR ?= /lib/modules/`uname -r`/build

modules:
	@$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	@$(MAKE) -C $(KDIR) M=$$PWD clean

install:
	@$(MAKE) -C $(KDIR) M=$$PWD modules_install
	@depmod -a

endif

