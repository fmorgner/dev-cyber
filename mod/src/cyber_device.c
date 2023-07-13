/*
 * cyber - Add CYBER to your system
 * Copyright (C) 2017 Felix Morgner
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cyber.h"
#include "cyber_device.h"

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>

/*
 * UDEV 
 */

typedef struct device *device_ptr;
typedef struct device const *const_device_ptr;
typedef struct kobj_uevent_env *uevent_env_ptr;
typedef struct kobj_uevent_env const *const_uevent_env_ptr;

static int _handle_uevent(const_device_ptr, uevent_env_ptr environment)
{
	return add_uevent_var(environment, "DEVMODE=%#o", 0666);
}

/*
 * Device Class
 */

typedef struct class *class_ptr;
typedef struct class const *const_class_ptr;
typedef struct class_attribute *attribute_ptr;
typedef struct class_attribute const *const_attribute_ptr;

/// Get the amount of available cyber.
static ssize_t available_show(const_class_ptr, const_attribute_ptr, char * __user output_buffer)
{
	return sprintf(output_buffer, "infinite\n");
}

/// Get the storage technology in use.
static ssize_t storage_technology_show(const_class_ptr, const_attribute_ptr, char * __user output_buffer)
{
	return sprintf(output_buffer, "condensed di-hydrogen-monoxide\n");
}

static CLASS_ATTR_RO(available);
static CLASS_ATTR_RO(storage_technology);

static int _init_device_class(cyber_device * device)
{
	int error = 0;

	device->class.name = CLS_NAME;
	device->class.dev_uevent = _handle_uevent;

	if((error = class_register(&device->class)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to register CYBER device class!\n");
		return error;
	}

	if((error = class_create_file(&device->class, &class_attr_available)) || (error = class_create_file(&device->class, &class_attr_storage_technology)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to register sysfs CYBER attributes!\n");
		return error;
	}

	return 0;
}

static void _shutdown_device_class(cyber_device * device)
{
	class_unregister(&device->class);
}

/// Character Device

typedef struct file_operations *file_operations_ptr;
typedef struct file_operations const *const_file_operations_ptr;

static int _init_character_device(cyber_device * device, const_file_operations_ptr file_ops)
{
	int error = 0;

	if((error = alloc_chrdev_region(&device->number, 0, 1, DEV_NAME)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to allocate character device!\n");
	}

	cdev_init(&device->character, file_ops);
	device->character.owner = THIS_MODULE;
	device->character.ops = file_ops;

	if((error = cdev_add(&device->character, device->number, 1)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to add character device (Error: %d)!\n", error);
	}

	return error;
}

static void _shutdown_character_device(cyber_device * device)
{
	cdev_del(&device->character);
	unregister_chrdev_region(device->number, 1);
}

/// Kernel Device

static void _close_kernel_device(struct device * device)
{
	printk(KERN_INFO DEV_NAME ": Closing kernel CYBER device\n");
}

static int _init_kernel_device(cyber_device * device)
{
	int error = 0;

	device->kernel.class = &device->class;
	device->kernel.devt = device->number;
	device->kernel.init_name = DEV_NAME;
	device->kernel.release = _close_kernel_device;

	if((error = device_register(&device->kernel)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to create CYBER device!\n");
	}

	return error;
}

static void _shutdown_kernel_device(cyber_device * device)
{
	device_unregister(&device->kernel);
}

/// Device

int cyber_device_init(cyber_device * device, const_file_operations_ptr file_ops)
{
	int error = 0;

	printk(KERN_INFO DEV_NAME ": Initializing CYBER device\n");

	if((error = _init_character_device(device, file_ops)))
	{
		return error;
	}

	if((error = _init_device_class(device)))
	{
		_shutdown_character_device(device);
		return error;
	}

	if((error = _init_kernel_device(device)))
	{
		_shutdown_character_device(device);
		_shutdown_device_class(device);
		return error;
	}

	printk(KERN_INFO DEV_NAME ": New CYBER device with major %d minor %d\n", MAJOR(device->number), MINOR(device->number));
	return error;
}

void cyber_device_shutdown(cyber_device * device)
{
	printk(KERN_INFO DEV_NAME ": Shutting down CYBER device\n");
	_shutdown_kernel_device(device);
	_shutdown_device_class(device);
	_shutdown_character_device(device);
}

void cyber_device_release(struct device * device)
{
	printk(KERN_INFO DEV_NAME ": Closing kernel CYBER device\n");
}
