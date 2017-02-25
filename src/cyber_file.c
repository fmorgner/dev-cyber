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
#include "cyber_file.h"

#include <asm/uaccess.h>
#include <linux/module.h>

struct file_operations const cyber_operations = {
	.owner = THIS_MODULE,
	.open = cyber_file_open,
	.read = cyber_file_read,
	.write = cyber_file_write,
	.release = cyber_file_close,
};

static char const * cyberPattern = "!CYBER! ";
static char * cyberSpace;

int cyber_file_open(struct inode * inode, struct file * file)
{
	return 0;
}

ssize_t cyber_file_read(struct file * file, char __user * buffer, size_t size, loff_t * offset)
{
	int const cybers = (size > PAGE_SIZE ? PAGE_SIZE : size) / 8;
	if(copy_to_user(buffer, cyberSpace, cybers * 8))
	{
		return -EFAULT;
	}
	return cybers * 8;
}

ssize_t cyber_file_write(struct file * file, char __user const * buffer, size_t size, loff_t * offset)
{
	return size;
}

int cyber_file_close(struct inode * inode, struct file * file)
{
	return 0;
}

int cyber_file_init(void)
{
	int i;

	printk(KERN_INFO DEV_NAME ": Initializing CYBER space\n");
	cyberSpace = (char *)__get_free_page(GFP_KERNEL);
	if(!cyberSpace)
	{
		printk(KERN_ALERT DEV_NAME ": Out of CYBER space!\n");
		return ENOMEM;
	}

	for(i = 0; i < PAGE_SIZE; ++i)
	{
		cyberSpace[i] = cyberPattern[i % 8];
	}

	return 0;
}

void cyber_file_shutdown(void)
{
	printk(KERN_INFO DEV_NAME ": Releasing CYBER space\n");
	free_page((unsigned long)cyberSpace);
}
