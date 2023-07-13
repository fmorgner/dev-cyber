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

#include "cyber_device.h"
#include "cyber_file.h"
#include "cyber_lifecycle.h"

#include <linux/module.h>
#include <linux/fs.h>

cyber_device device = {};
struct file_operations file_ops = {.owner = THIS_MODULE};

int cyber_driver_init(void)
{
	int error;

	if((error = cyber_file_init(&file_ops)))
	{
		return error;
	}

	if((error = cyber_device_init(&device, &file_ops)) < 0)
	{
		cyber_file_shutdown();
		return error;
	}

	return 0;
}

void cyber_driver_exit(void)
{
	cyber_device_shutdown(&device);
	cyber_file_shutdown();
}

module_init(cyber_driver_init)
module_exit(cyber_driver_exit)
