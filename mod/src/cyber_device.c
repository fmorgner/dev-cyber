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
#include <linux/module.h>
#include <linux/string.h>

struct cyber_device device = {};

/**
 * Initialize the sysfs CYBER device class
 *
 * @return zero on success, non-zero otherwise
 */
int cyber_device_init_device_class(void);

/**
 * Shutdown the sysfs CYBER device class
 */
void cyber_device_shutdown_device_class(void);

/**
 * Initialize the CYBER character device
 *
 * @return zero on success, non-zero otherwise
 */
int cyber_device_init_character_device(void);

/**
 * Shutdown the CYBER character device
 */
void cyber_device_shutdown_character_device(void);

/**
 * Initialize the CYBER kernel device
 *
 * @return zero on success, non-zero otherwise
 */
int cyber_device_init_kernel_device(void);

/**
 * Shutdown the CYBER kernel device
 */
void cyber_device_shutdown_kernel_device(void);

int cyber_device_init(void)
{
	int error = 0;

	printk(KERN_INFO DEV_NAME ": Initializing CYBER device\n");

	if((error = cyber_device_init_character_device()))
	{
		return error;
	}

	if((error = cyber_device_init_device_class()))
	{
		cyber_device_shutdown_character_device();
		return error;
	}

	if((error = cyber_device_init_kernel_device()))
	{
		cyber_device_shutdown_character_device();
		cyber_device_shutdown_device_class();
		return error;
	}

	printk(KERN_INFO DEV_NAME ": New CYBER device with major %d minor %d\n", MAJOR(device.number), MINOR(device.number));
	return error;
}

void cyber_device_shutdown(void)
{
	printk(KERN_INFO DEV_NAME ": Shutting down CYBER device\n");
	cyber_device_shutdown_kernel_device();
	cyber_device_shutdown_device_class();
	cyber_device_shutdown_character_device();
}

void cyber_device_release(struct device * device)
{
	printk(KERN_INFO DEV_NAME ": Closing kernel CYBER device\n");
}
