EXTRA_CFLAGS := -I$(src)/include

obj-m := cyber.o

cyber-objs :=	src/cyber_device.o \
							src/cyber_device_character_device.o \
							src/cyber_device_device_class.o \
							src/cyber_device_kernel_device.o \
							src/cyber_file.o \
							src/cyber_lifecycle.o \
							src/cyber.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules

install:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules_install
	depmod -a

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
