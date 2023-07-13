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

#ifndef CYBER_DEVICE_H
#define CYBER_DEVICE_H

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

/**
 * This structure holds information about the CYBER device
 */
typedef struct
{
	/**
	 * The kernel character device structure
	 */
	struct cdev character;

	/**
	 * The kernel device structure
	 */
	struct device kernel;

	/**
	 * The kernel device class structure
	 */
	struct class class;

	/**
	 * The kernel device number
	 */
	dev_t number;

	/**
	 * Cyber space!
	 */
	char * space;

} cyber_device;

/**
 * Initialize the CYBER device
 */
int cyber_device_init(cyber_device * device, struct file_operations const * file_ops);

/**
 * Deinitialize the CYBER device
 */
void cyber_device_shutdown(cyber_device * device);

#endif
