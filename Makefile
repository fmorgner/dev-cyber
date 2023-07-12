EXTRA_CFLAGS := -I$(src)/include

obj-m := cyber.o

cyber-objs :=	src/cyber_device.o \
							src/cyber_device_character_device.o \
							src/cyber_device_device_class.o \
							src/cyber_device_kernel_device.o \
							src/cyber_file.o \
							src/cyber_lifecycle.o \
							src/cyber.o

ifeq ($(KERNEL_VERSION),)
KERNEL_VERSION=$(shell uname -r)
endif

all: check
	make -C /lib/modules/$(KERNEL_VERSION)/build/ M=$(PWD) modules

install: check
	make -C /lib/modules/$(KERNEL_VERSION)/build/ M=$(PWD) modules_install
	depmod -a

clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build/ M=$(PWD) clean

check:
ifeq ($(wildcard /lib/modules/$(KERNEL_VERSION)/build/.),)
	@echo "did you install the \"linux-headers\" package?"
endif
