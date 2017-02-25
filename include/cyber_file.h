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

#ifndef CYBER_FILE_H
#define CYBER_FILE_H

#include <linux/fs.h>

/**
 * Initialize the CYBER file
 *
 * @return zero on success, non-zero otherwise
 */
int cyber_file_init(void);

/**
 * Shutdown the CYBER file
 */
void cyber_file_shutdown(void);

/**
 * Handler for CYBER device open events
 *
 * @param inode The kernel inode associated with the CYBER device
 * @param file The kernel file associated with the CYBER device
 * @return zero on success, non-zero otherwise
 */
int cyber_file_open(struct inode * inode, struct file * file);

/**
 * Handler for CYBER device read events
 *
 * @param file The kernel file associated with the CYBER device
 * @param buffer The user-space target buffer for all the CYBER
 * @param size The size of the user-space buffer
 * @param offset The offset into the CYBER
 * @return The number of bytes that were read, negative on error
 */
ssize_t cyber_file_read(struct file * file, char * buffer, size_t size, loff_t * offset);

/**
 * Handler for CYBER device write events
 *
 * @param file The kernel file associated with the CYBER device
 * @param buffer The user-space source buffer for all the CYBER
 * @param size The size of the user-space buffer
 * @param offset The offset into the CYBER
 * @param The number of bytes that were written, negative on error
 */
ssize_t cyber_file_write(struct file * file, char const * buffer, size_t size, loff_t * offset);

/**
 * Handler for CYBER device close events
 *
 * @param inode The kernel inode associated with the CYBER device
 * @param file The kernel file associated with the CYBER device
 * @param zero on success, non-zero otherwise
 */
int cyber_file_close(struct inode * inode, struct file * file);

#endif
