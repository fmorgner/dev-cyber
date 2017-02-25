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

static void cyber_device_close_kernel_device(struct device * device)
{
	printk(KERN_INFO DEV_NAME ": Closing kernel CYBER device\n");
}

int cyber_device_init_kernel_device(void)
{
	int error = 0;

	device.kernel.class = &device.class;
	device.kernel.devt = device.number;
	device.kernel.init_name = DEV_NAME;
	device.kernel.release = cyber_device_close_kernel_device;

	if((error = device_register(&device.kernel)))
	{
		printk(KERN_ALERT DEV_NAME ": Failed to create CYBER device!\n");
	}

	return error;
}

void cyber_device_shutdown_kernel_device(void)
{
	device_unregister(&device.kernel);
}
