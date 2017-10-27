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
#include <linux/fs.h>
#include <linux/mm.h>

static char const * cyberPattern = "!CYBER! ";
static char * cyberSpace;

/**
 * Handler for CYBER device open events
 *
 * @param inode The kernel inode associated with the CYBER device
 * @param file The kernel file associated with the CYBER device
 * @return zero on success, non-zero otherwise
 */
static int cyber_file_open(struct inode * inode, struct file * file)
{
	return 0;
}

/**
 * Handler for CYBER device read events
 *
 * @param file The kernel file associated with the CYBER device
 * @param buffer The user-space target buffer for all the CYBER
 * @param size The size of the user-space buffer
 * @param offset The offset into the CYBER
 * @return The number of bytes that were read, negative on error
 */
static ssize_t cyber_file_read(struct file * file, char __user * buffer, size_t size, loff_t * offset)
{
	int const cyberChunks = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	int const cybersPerChunk = (size > PAGE_SIZE ? PAGE_SIZE : size) / 8;
	int copiedCybers = 0;

	for(; copiedCybers < cyberChunks; ++copiedCybers)
	{
		if(raw_copy_to_user(buffer + copiedCybers * PAGE_SIZE, cyberSpace, cybersPerChunk * 8))
		{
			return -EFAULT;
		}
	}

	return cyberChunks * cybersPerChunk * 8;
}

static int cyber_file_mmap(struct file * file, struct vm_area_struct * vma)
{
	ssize_t vmaSize = vma->vm_end - vma->vm_start;
	ssize_t offset = 0;
	for(; vmaSize; vmaSize -= PAGE_SIZE, ++offset)
	{
		vm_insert_page(vma, vma->vm_start + offset * PAGE_SIZE, virt_to_page(cyberSpace));
	}
	return 0;
}

/**
 * Handler for CYBER device write events
 *
 * @param file The kernel file associated with the CYBER device
 * @param buffer The user-space source buffer for all the CYBER
 * @param size The size of the user-space buffer
 * @param offset The offset into the CYBER
 * @param The number of bytes that were written, negative on error
 */
static ssize_t cyber_file_write(struct file * file, char __user const * buffer, size_t size, loff_t * offset)
{
	return size;
}

/**
 * Handler for CYBER device close events
 *
 * @param inode The kernel inode associated with the CYBER device
 * @param file The kernel file associated with the CYBER device
 * @param zero on success, non-zero otherwise
 */
static int cyber_file_close(struct inode * inode, struct file * file)
{
	return 0;
}

struct file_operations const cyber_operations = {
	.owner = THIS_MODULE,
	.open = cyber_file_open,
	.read = cyber_file_read,
	.write = cyber_file_write,
	.release = cyber_file_close,
	.mmap = cyber_file_mmap,
};

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
