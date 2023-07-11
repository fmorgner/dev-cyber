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

extern struct cyber_device device;
extern struct file_operations const cyber_operations;

int cyber_device_init_character_device(void)
{
	int error = 0;

	if((error = alloc_chrdev_region(&device.number, 0, 1, DEV_NAME)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to allocate character device!\n");
	}

	cdev_init(&device.character, &cyber_operations);
	device.character.owner = THIS_MODULE;
	device.character.ops = &cyber_operations;

	if((error = cdev_add(&device.character, device.number, 1)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to add character device (Error: %d)!\n", error);
	}

	return error;
}

void cyber_device_shutdown_character_device(void)
{
	cdev_del(&device.character);
	unregister_chrdev_region(device.number, 1);
}
