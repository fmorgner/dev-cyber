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

#endif
