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
 * FITNESS FOR A PARTICULAR PURPOSE.	See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.	If not, see <http://www.gnu.org/licenses/>.
 */

#include "cyber.h"
#include "cyber_device.h"

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

extern struct cyber_device device;

static ssize_t available_show(struct class * class, struct class_attribute * attribute, char * __user buffer)
	{
	return sprintf(buffer, "infinite\n");
	}
static CLASS_ATTR_RO(available);

static ssize_t storage_technology_show(struct class * class, struct class_attribute * attribute, char * __user buffer)
	{
	return sprintf(buffer, "condensed di-hydrogen-monoxide\n");
	}
static CLASS_ATTR_RO(storage_technology);

static int cyber_device_handle_uevent(struct device * kernelDevice, struct kobj_uevent_env * environment)
{
	add_uevent_var(environment, "DEVMODE=%#o", 0666);
	return 0;
}

int cyber_device_init_device_class(void)
{
	int error = 0;

	device.class.name = CLS_NAME;
	device.class.owner = THIS_MODULE;
	device.class.dev_uevent = cyber_device_handle_uevent;
	if((error = class_register(&device.class)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to register CYBER device class!\n");
	}

	if((error = class_create_file(&device.class, &class_attr_available)) ||
		 (error = class_create_file(&device.class, &class_attr_storage_technology)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to register sysfs CYBER attributes!\n");
	}

	return error;
}

void cyber_device_shutdown_device_class(void)
{
	class_unregister(&device.class);
}
